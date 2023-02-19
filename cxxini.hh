/*
 * cxxini - A C++ INI reading and writing library
 * https://github.com/LordOfTrident/cxxini
 */

#ifndef CXXINI_HH_HEADER_GUARD
#define CXXINI_HH_HEADER_GUARD

#include <string>           // std::string
#include <map>              // std::map
#include <iostream>         // std::istream, std::ostream
#include <initializer_list> // std::initializer_list
#include <cctype>           // std::isspace

#define CXXINI_VERSION_MAJOR 1
#define CXXINI_VERSION_MINOR 0
#define CXXINI_VERSION_PATCH 0

namespace ini {
	constexpr const char *default_section = "";

	using section  = std::map<std::string, std::string>;
	using sections = std::map<std::string, section>;

	class record : public sections {
	public:
		record() {}
		record(const sections &sec): sections(sec) {}

		record(const std::string &str) {parse(str);}
		record(std::istream &stream)   {parse(stream);}

		std::string stringify() const {
			std::string s;
			bool        first = true, newline = false;
			for (const auto &section : *this) {
				if (not first or section.first != default_section) {
					if (newline)
						s += "\n\n";
					else
						newline = true;
					s += "[" + section.first + "]";
				}

				if (first)
					first = false;

				for (const auto &pair : section.second) {
					if (newline)
						s += "\n";
					else
						newline = true;

					s += stringify_value(pair.first) + " = " + stringify_value(pair.second);
				}
			}
			return s;
		}

		size_t parse(const std::string &str) {
			parsing_section.clear();

			size_t pos = 0, row = 1;
			while (pos < str.length()) {
				size_t next = str.find_first_of('\n', pos);

				std::string line;
				if (next == std::string::npos)
					line = str.substr(pos);
				else
					line = str.substr(pos, next - pos);

				if (not parse_line(line))
					return row;

				if (next == std::string::npos)
					break;

				pos = next + 1;
				++ row;
			}
			return 0;
		}

		size_t parse(std::istream &stream) {
			parsing_section.clear();

			size_t      row = 1;
			std::string line;
			while (std::getline(stream, line)) {
				if (not parse_line(line))
					return row;

				++ row;
			}
			return 0;
		}

		friend std::ostream &operator <<(std::ostream &stream, const record &rec) {
			stream << rec.stringify();
			return stream;
		}

		friend std::istream &operator >>(std::istream &stream, record &rec) {
			rec.parse(stream);
			return stream;
		}

	private:
		std::string parsing_section;

		void unquote(std::string &str) const {
			if (str[0] == '"' and str[str.length() - 1] == '"')
				str = str.substr(1, str.length() - 2);
		}

		void trim_right(std::string &str, const std::string &to_trim) const {
			str.erase(str.find_last_not_of(to_trim) + 1);
		}

		void trim_left(std::string &str, const std::string &to_trim) const {
			str.erase(0, str.find_first_not_of(to_trim));
		}

		void trim(std::string &str, const std::string &to_trim) const {
			trim_right(str, to_trim);
			trim_left( str, to_trim);
		}

		bool has_trailing_spaces(const std::string &str) const {
			return std::isspace(str[0]) or std::isspace(str[str.length() - 1]);
		}

		std::string unescape_value(const std::string &val) {
			std::string s;
			bool        escape = false;
			for (auto ch : val) {
				if (ch == '\\' and not escape)
					escape = true;
				else if (escape) {
					escape = false;
					s += ch;
				} else
					s += ch;
			}
			return s;
		}

		bool parse_line(const std::string &line) {
			constexpr const char *whitespaces = " \f\n\r\t\v";

			size_t pos = line.find_first_not_of(whitespaces);
			if (pos == std::string::npos)
				return true;

			switch (line[pos]) {
			case '#': case ';': return true;
			case '[': {
				++ pos;
				size_t end = line.find_first_of(']', pos);
				if (end == std::string::npos)
					return false;

				parsing_section = line.substr(pos, end - pos);
				(*this)[parsing_section];
			} break;

			case '=': return false;

			default: {
				size_t split  = pos;
				bool   escape = false;
				while (true) {
					if (line[split] == '\\' and not escape)
						escape = true;
					else if (escape)
						escape = false;

					if (line[split] == '=' and not escape)
						break;

					++ split;
					if (split >= line.length())
						return false;
				}

				std::string key = line.substr(pos, split - pos);
				std::string val = line.substr(split + 1);
				if (key.length() == 0 or val.length() == 0)
					return false;

				trim(key, whitespaces);
				unquote(key);
				key = unescape_value(key);

				trim(val, whitespaces);
				unquote(val);
				val = unescape_value(val);

				(*this)[parsing_section][key] = val;
			} break;
			}

			return true;
		}

		std::string stringify_value(const std::string &val) const {
			std::string s;

			bool quote = has_trailing_spaces(val);
			if (quote)
				s += "\"";

			for (auto ch : val) {
				switch (ch) {
				case '=':  s += "\\=";  break;
				case '\\': s += "\\\\"; break;
				case '"':  s += "\\\""; break;
				default:   s += ch;
				}
			}

			if (quote)
				s += "\"";

			return s;
		}
	};
}

#endif
