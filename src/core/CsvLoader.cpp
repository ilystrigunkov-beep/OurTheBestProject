#include "core/CsvLoader.hpp"

#include "core/Exceptions.hpp"

#include <algorithm>
#include <chrono>
#include <charconv>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace core {

namespace {

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;

    for (char ch : line) {
        if (ch == '"') {
            in_quotes = !in_quotes;
            current.push_back(ch);
        } else if (ch == delimiter && !in_quotes) {
            tokens.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    tokens.push_back(current);

    return tokens;
}

double parse_double(const std::string& text, const std::string& field_name) {
    std::string s = text;

    // Trim whitespace.
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.erase(s.begin());
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }

    // Strip surrounding quotes.
    if (s.size() >= 2 && ((s.front() == '"' && s.back() == '"') ||
                          (s.front() == '\'' && s.back() == '\''))) {
        s = s.substr(1, s.size() - 2);
    }

    // Remove thousands separators and percent sign.
    s.erase(std::remove(s.begin(), s.end(), ','), s.end());
    s.erase(std::remove(s.begin(), s.end(), '%'), s.end());

    if (s.empty()) {
        return 0.0;
    }

    try {
        return std::stod(s);
    } catch (const std::exception&) {
        throw ParseException("Failed to parse field '" + field_name + "': '" + text + "'");
    }
}

std::optional<long long> parse_int64(std::string_view text) {
    long long value{};
    auto* begin = text.data();
    auto* end = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(begin, end, value);
    if (ec != std::errc{} || ptr != end) {
        return std::nullopt;
    }
    return value;
}

bool looks_like_header_token(std::string_view token) {
    for (char ch : token) {
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            return true;
        }
    }
    return false;
}

// Howard Hinnant's days_from_civil algorithm (public domain)
long long days_from_civil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;     // [0, 146096]
    return static_cast<long long>(era) * 146097 + static_cast<long long>(doe) - 719468;
}

std::optional<long long> parse_date_to_unix(std::string_view text) {
    std::string s(text);

    // Trim whitespace
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.erase(s.begin());
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }

    // Strip quotes
    if (s.size() >= 2 && ((s.front() == '"' && s.back() == '"') ||
                          (s.front() == '\'' && s.back() == '\''))) {
        s = s.substr(1, s.size() - 2);
    }

    // Expect "MM/DD/YYYY"
    const auto first_sep = s.find('/');
    const auto second_sep = s.find('/', first_sep == std::string::npos ? 0 : first_sep + 1);
    if (first_sep == std::string::npos || second_sep == std::string::npos) {
        return std::nullopt;
    }

    try {
        const int month = std::stoi(s.substr(0, first_sep));
        const int day = std::stoi(s.substr(first_sep + 1, second_sep - first_sep - 1));
        const int year = std::stoi(s.substr(second_sep + 1));
        const long long days = days_from_civil(year, static_cast<unsigned>(month), static_cast<unsigned>(day));
        return days * 86400LL;
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace

std::unique_ptr<TimeSeries> CsvLoader::load(const std::string& path, char delimiter) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw FileException("Failed to open CSV file: " + path);
    }

    std::vector<Candle> candles;
    std::string line;

    // Read first line (could be header or data)
    if (!std::getline(in, line)) {
        return std::make_unique<TimeSeries>(std::move(candles));
    }

    auto header_tokens = split(line, delimiter);
    bool has_header = !header_tokens.empty() && looks_like_header_token(header_tokens[0]);

    // Determine column indices for the common Investing.com "S&P 500 Historical Data" layout:
    // "Date","Price","Open","High","Low","Vol.","Change %"
    int idx_time = 0;
    int idx_open = 1;
    int idx_high = 2;
    int idx_low  = 3;
    int idx_close = 4;
    int idx_volume = -1;

    if (has_header) {
        const auto col_count = static_cast<int>(header_tokens.size());
        auto find_col = [&](std::string_view name) -> int {
            for (int i = 0; i < col_count; ++i) {
                auto t = header_tokens[static_cast<std::size_t>(i)];
                if (t.find(name) != std::string::npos) {
                    return i;
                }
            }
            return -1;
        };

        int date_col = find_col("Date");
        int price_col = find_col("Price");
        int open_col = find_col("Open");
        int high_col = find_col("High");
        int low_col = find_col("Low");
        int vol_col = find_col("Vol");

        if (date_col >= 0 && price_col >= 0 && open_col >= 0 &&
            high_col >= 0 && low_col >= 0) {
            idx_time = date_col;
            // In this layout "Price" is actually the close.
            idx_close = price_col;
            idx_open = open_col;
            idx_high = high_col;
            idx_low = low_col;
            idx_volume = vol_col;
        }
    } else {
        // First line is actually data – treat it with default layout:
        // timestamp,open,high,low,close[,volume]
        auto tokens = std::move(header_tokens);
        if (tokens.size() < 5) {
            throw ParseException("CSV line has fewer than 5 fields: " + line);
        }

        const auto maybe_ts = parse_int64(tokens[0]);
        const long long ts_seconds = maybe_ts.value_or(0LL);
        auto ts = Candle::TimePoint{std::chrono::seconds(ts_seconds)};
        double open = parse_double(tokens[1], "open");
        double high = parse_double(tokens[2], "high");
        double low = parse_double(tokens[3], "low");
        double close = parse_double(tokens[4], "close");
        std::optional<double> volume;
        if (tokens.size() > 5 && !tokens[5].empty()) {
            volume = parse_double(tokens[5], "volume");
        }
        candles.emplace_back(ts, open, high, low, close, volume);
    }

    std::size_t row_index = candles.size();
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        auto tokens = split(line, delimiter);
        if (tokens.size() < 5) {
            throw ParseException("CSV line has fewer than 5 fields: " + line);
        }

        const auto& time_token = tokens[static_cast<std::size_t>(idx_time)];
        auto maybe_ts = parse_int64(time_token);
        if (!maybe_ts) {
            maybe_ts = parse_date_to_unix(time_token);
        }
        const long long ts_seconds = maybe_ts.value_or(static_cast<long long>(row_index));
        auto ts = Candle::TimePoint{std::chrono::seconds(ts_seconds)};
        double open = parse_double(tokens[static_cast<std::size_t>(idx_open)], "open");
        double high = parse_double(tokens[static_cast<std::size_t>(idx_high)], "high");
        double low = parse_double(tokens[static_cast<std::size_t>(idx_low)], "low");
        double close = parse_double(tokens[static_cast<std::size_t>(idx_close)], "close");
        std::optional<double> volume;
        if (idx_volume >= 0 &&
            static_cast<std::size_t>(idx_volume) < tokens.size() &&
            !tokens[static_cast<std::size_t>(idx_volume)].empty()) {
            volume = parse_double(tokens[static_cast<std::size_t>(idx_volume)], "volume");
        }

        candles.emplace_back(ts, open, high, low, close, volume);
        ++row_index;
    }

    return std::make_unique<TimeSeries>(std::move(candles));
}

} // namespace core

