#include <string_view>
using std::string_view;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include <algorithm>
#include <iterator>
#include <iomanip>

#include "tip.h"

/// Error messages
constexpr std::string_view err_msg[]{
    "No errors.",
    "Cannot open input file.",
    "Found a field with no value",
    "Found an invalid field declaration",
    "Found an invalid field name",
    "Requested field could not be found in this section.",
    "Section non existing.",
    "Invalid integer value.",
    "Invalid real value.",
    "Invalid boolean value.",
};


//==============================================================//
//== These are auxiliary functions to help us parse the file. ==//
//--------------------------------------------------------------//

/// Types of "white spaces", we need to remove.
constexpr std::string_view WHITESPACE = " \n\r\t\f\v";

/// Types of char to be treated as comment both at the beginning of a line or in the middle of it.
constexpr std::string_view COMMENT = ";";

/// Lambda expression that transform the string to lowercase.
auto TO_LOWERCASE = [](const std::string& s)->std::string {
    std::string str{ s };
    std::transform( str.begin(), str.end(), str.begin(), ::tolower );
    return str;
};

/// Remove all quotation marks on a string.
auto REMOVE_QUOTES = [](const std::string& s)->std::string {
    std::string str;
    std::copy_if(s.cbegin(),s.cend(),
                 std::back_inserter(str),
                 [](unsigned char c) { return c != '"'; });
    return str;
};

/// Remove all square brackets from a string.
auto REMOVE_SQUARE_BRACKETS= [](const std::string& s)->std::string {
    std::string str;
    std::copy_if(s.cbegin(),s.cend(),
                 std::back_inserter(str),
                 [](unsigned char c) { return (c!='[' and c!=']'); });
    return str;
};

/// Trim all content after a comment characters (included) located in a string
auto REMOVE_COMMENTS= [](const std::string &s)->std::string {
    size_t end = s.find_first_of(COMMENT);
    return (end == std::string::npos) ? s : s.substr(0, end);
};

/// Remove all leading white spaces from a string.
auto LTRIM = [](const std::string &s)->std::string {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
};

/// Remove all trailing white spaces from a string.
auto RTRIM = [](const std::string &s)->std::string {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
};

/// Remove all leading/trailing white spaces from a string.
auto TRIM_WS = [](const std::string &s)->std::string {
    return RTRIM(LTRIM(s));
};

/// Tokenize a string into componentes separated by a delimiter.
/*!
 * Split a string into a list of components (tokens) which were separeted
 * by the `delimiter` in the original string.
 * \note White spaces are removed.
 *
 * @param input_str The input string.
 * @param delimiter The delimiter symbol.
 * @return A list of tokenized strings.
 */
std::vector<std::string> split(const std::string & input_str, char delimiter=' '){
    // Store the tokens.
    std::vector<std::string> tokens;
    // read tokens from a string buffer.
    std::istringstream iss;
    iss.str(input_str);
    // This will hold a single token temporarily.
    std::string token;
    while (std::getline(iss >> std::ws, token, delimiter))
        tokens.emplace_back(token);
    return tokens;
}


//===============================================//
//== These are methods of the TIP class. ==//
//-----------------------------------------------//


TIP::TIP(const std::string_view& filename)
    : m_error_code{OK},
      m_section_line{0},
      m_field_line{0}
{
    std::fstream ifs(filename.data());
    // Check if openning was successful.
    if(not ifs.is_open()){
        m_error_code = code_t::CANNOT_OPEN_FILE;
        return;
    }
    // Let us parse each line from the input file.
    std::string line;
    size_t line_number{0};
    std::string curr_section_name{"ROOT"}; // Default section name.
    while(std::getline(ifs, line)){ // remove leading white spaces.
        // [1]: remove comments and surrounding spaces.
        auto s_line = TRIM_WS(REMOVE_COMMENTS(line));
        // [2]: Always increment line count (to support more precise error messages)
        line_number++;
        // [3]: Skip empty lines or comment lines.
        if(s_line.empty()){
            ; // Get next line from file.
        } else if(s_line[0]=='['){
            // [4]: Looking for a section indication.
            // Keep track of the line the section begins at.
            m_section_line = line_number;
            // Strip line and remove indicators of a section.
            curr_section_name = TO_LOWERCASE(REMOVE_SQUARE_BRACKETS(s_line));
            // Create a new (empty) Hash Map associated with this section.
            m_sections[curr_section_name] = HashMap{};
        } else {
            // [5]: Parsing a field.
            m_field_line = line_number;
            auto stripped_line = REMOVE_QUOTES(s_line);
            auto tokens = split(stripped_line,'='); // Break up field and value.
            if(tokens.empty() or tokens.size()>2) {
                m_error_code = code_t::INVALID_FIELD; // Too little or too many information.
            } else if(tokens.size()==1){ // We've just got the field, no value though.
                m_error_code = code_t::INCOMPLETE_FIELD;
            } else { // Got exactly 2 tokens.
                // Let us check if the field name has only one word (i.e. no spaces between them).
                auto tokens_parts = split(tokens[0],' ');
                if(tokens_parts.size()!=1){
                    m_error_code = code_t::INVALID_FIELD_NAME;
                } else { // Everything ok. Store it!
                    // Retrieve hash map associated w/ the current section
                    HashMap &m = m_sections[curr_section_name];
                    // Remember to strip tokens off of any leading/trailing spaces.
                    m[TRIM_WS(tokens[0])] = TRIM_WS(tokens[1]); // Store pair <field,value> in the map.
                }
            }
        }
        // [6]: Check if we got a error code.
        if(m_error_code!=code_t::OK)
            break; // Stop here.
    }
    ifs.close();
}

std::string
TIP::parser_error_msg(void) const{
    std::ostringstream oss;
    // Basic message.
    oss << err_msg[m_error_code];
    // Add line informarion, if this is the case.
    if(m_error_code==INCOMPLETE_FIELD or
       m_error_code==INVALID_FIELD_NAME or
       m_error_code==INVALID_INT_VALUE or
       m_error_code==INVALID_BOOL_VALUE or
       m_error_code==INVALID_REAL_VALUE or
       m_error_code==INVALID_FIELD){
        oss << ", at line " << m_field_line << ".";
    }
    return oss.str();
}

/// Returns a list of field names found in the database associated with a given section name.
std::vector<std::string> TIP::get_fields_list(const std::string& section_) const{
    std::vector<std::string> fields;
    // Does this section exist?
    if(m_sections.count(section_)!=0){
        // Capture all fields of this particular section.
        for(const auto&[field, value] : m_sections.at(section_))
            fields.emplace_back(field);
    }
    return fields;
}

/// Returns a list of section names found in the database.
std::vector<std::string> TIP::get_sections_list(void) const{
    std::vector<std::string> sections;
    // Capture all the sections from this file.
    for(const auto&[sec_name, hmap] : m_sections)
        sections.emplace_back(sec_name);
    return sections;
}

/*!
 * Retrive from the INI database a **string** value associated
 * with the Field `field_`, defined in Section `section_`.
 *
 * In case the retrieving proccess fails the fallback
 * value is returned, along with an error code.
 *
 * @param section_ The section name.
 * @param field_ The field name inside the section.
 * @param fbv_ The fall back value.
 * @return The **string** value of the requested field, or the fall back value in case an error is detected.
 * @see retrieve_value()
 */
std::string TIP::get_str(const string& section_, const string& field_, const string& fbv_) {
    // Fall back value in case the retrieving failed.
    auto value{fbv_};
    // Retrieve the field value as a plain string.
    auto str_val= retrieve_value(section_, field_);
    // Have we got a value?
    if(str_val.has_value()){
        // Try to convert string to the expected type.
        value = str_val.value();
    }
    return value;
}

/*!
 * Retrive from the INI database an **integer** value associated
 * with the Field `field_`, defined in Section `section_`.
 *
 * In case the retrieving proccess fails the fallback
 * value is returned, along with an error code.
 *
 * @param section_ The section name.
 * @param field_ The field name inside the section.
 * @param fbv_ The fall back value.
 * @return The **int** value of the requested field, or the fall back value in case an error is detected.
 * @see retrieve_value()
 */
int TIP::get_int (const string& section_, const string& field_, int fbv_) {
    // Fall back value in case the retrieving failed.
    auto value{fbv_};
    // Retrieve the field value as a plain string.
    auto str_val= retrieve_value(section_, field_);
    // Have we got a value?
    if(str_val.has_value()){
        // Try to convert to integer.
        try { value = std::stoi(str_val.value()); }
        // Could not convert into a valid integer.
        catch(...){ m_error_code = code_t::INVALID_INT_VALUE; }
    }
    return value;
}

/*!
 * Retrive from the INI database a **real** value associated
 * with the Field `field_`, defined in Section `section_`.
 *
 * In case the retrieving proccess fails the fallback
 * value is returned, along with an error code.
 *
 * @param section_ The section name.
 * @param field_ The field name inside the section.
 * @param fbv_ The fall back value.
 * @return The **real** value of the requested field, or the fall back value in case an error is detected.
 * @see retrieve_value()
 */
TIP::real_t TIP::get_real(const string& section_, const string& field_, real_t fbv_) {
    // Fall back value in case the retrieving failed.
    auto value{fbv_};
    // Retrieve the field value as a plain string.
    auto str_val= retrieve_value(section_, field_);
    // Have we got a value?
    if(str_val.has_value()){
        // Try to convert the string into a real value.
        try { value = std::stof(str_val.value()); }
        // Could not convert into a valid integer.
        catch(...){ m_error_code = code_t::INVALID_REAL_VALUE; }
    }
    return value;
}

/*!
 * Retrive from the INI database a **bool** value associated
 * with the Field `field_`, defined in Section `section_`.
 *
 * In case the retrieving proccess fails the fallback
 * value is returned, along with an error code.
 *
 * @param section_ The section name.
 * @param field_ The field name inside the section.
 * @param fbv_ The fall back value.
 * @return The **bool** value of the requested field, or the fall back value in case an error is detected.
 * @see retrieve_value()
 */
bool TIP::get_bool(const string& section_, const string& field_, bool fbv_) {
    // Fall back value in case the retrieving failed.
    auto value{fbv_};
    // Retrieve the field value as a plain string.
    auto str_val= retrieve_value(section_, field_);
    // Have we got a value?
    if(str_val.has_value()){
        // Try to convert string to the expected type.
        if(str_val.value()=="true"){
            value = true;
        } else if(str_val.value()=="false"){
            value = false;
        } else { // Could not convert into a valid bool.
            m_error_code = code_t::INVALID_BOOL_VALUE;
        }
    }
    return value;
}

/*!
 * Retrive from the INI database a string value associated
 * with the Field `field_`, defined in Section `section_`.
 *
 * In case the section informed does not exist in the INI
 * database, a corresponding error code is generated.
 *
 * In case the field informed does not exist in the INI
 * database, a corresponding error code is generated.
 *
 * If an error is detected the method return an empty
 * optional value.
 *
 * @param section_ The section name.
 * @param field_ The field name inside the section.
 * @return The string value assigned to the field inside the section, or an empty value.
 */
std::optional<std::string> TIP::retrieve_value(const string& section_, const string& field_){
    // Reset the current error code.
    m_error_code = code_t::OK;
    // Does this section exist?
    if(m_sections.count(section_)==0){
        m_error_code = code_t::SECTION_NOT_FOUND;
    } else { // Section found, proceed...
        // Retrieve HashMap associated with this section name.
        const HashMap &hm = m_sections[section_];
        // Do we have the field name stored?
        if(hm.count(field_)==0){
            m_error_code = code_t::FIELD_NOT_FOUND;
        } else { // Ok, field value found.
            return TO_LOWERCASE(hm.at(field_));
        }
    }
    return {};
}

std::string TIP::pretty_print(void) const{
    std::ostringstream oss;
    // Get the section list so we can traverse their fields.
    auto sections = get_sections_list();
    for(const auto& sec_name : sections){
        // Lets determine the max field length. We need this to align all fields.
        size_t max_field_len{0};
        auto fields = get_fields_list(sec_name);
        std::for_each(fields.cbegin(),fields.cend(),
                [&](const string& s)
                { max_field_len = std::max(max_field_len,s.length()); });
        // Print the section name.
        oss << "[" << sec_name << "]\n";
        // Retrieve HashMap associated with this section name.
        const HashMap &hm = m_sections.at(sec_name);
        // Now print the fields, all right-algined;
        // the equal sign is placed at the same column in every line.
        for(const auto& field_name : fields)
            oss << "    "
                << std::setw(max_field_len) << field_name
                << " = "
                << std::quoted(hm.at(field_name)) << "\n";
        oss << '\n';
    }
    return oss.str();
}
