#ifndef TIP_H
#define TIP_H

#include <string>
using std::string;
#include <string_view>
using std::string_view;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <optional>
using std::optional;

/*
 * TODO:
 * [ ] Convert the class into a template.
 * [ ] Add namespace (if convertion happens).
 * [ ] Accept the real type as a template argument.
 * [ ] Accept the case insensitve flag via template.
 */

class TIP{
    public:
        /// Define the precision of the real type.
        using real_t = float;
        /// Another useful alias to reduce typing.
        using c_str_ref = const string&;
        /// List of possible errors the parser may yield.
        enum code_t {
            OK = 0,
            CANNOT_OPEN_FILE,  //!< Can't open input file.
            INCOMPLETE_FIELD,  //!< Field defined but no value assigned to.
            INVALID_FIELD,     //!< Something wrong in a field declaration.
            INVALID_FIELD_NAME,//!< Invalid field name (more than one word).
            FIELD_NOT_FOUND,   //!< The requested field-value could not be found in this section.
            SECTION_NOT_FOUND, //!< The requested section does not exist.
            INVALID_INT_VALUE, //!< Invalid integer value.
            INVALID_REAL_VALUE,//!< Invalid real value.
            INVALID_BOOL_VALUE,//!< Invalid boolean value.
        };

    private:
        /// Basic map of fields and values.
        typedef map<string,string> HashMap;
        //== Data members
        map<string, HashMap> m_sections; //!< List of all section entries.
        code_t m_error_code=OK;          //!< Current error code.
        size_t m_section_line=0;         //!< Keeps track of the current section line number
        size_t m_field_line=0;           //!< Keeps track of the current field line number

    private:
        /// Aux method that retrieves a string given a section/field pair of information.
        std::optional<string> retrieve_value(c_str_ref section, c_str_ref field);

    public:
        /// Ctro. Requires the INI filename as input.
        TIP(const string_view& file);
        /// Dtro.
        ~TIP()=default;
        /// Copy ctro is disabled.
        TIP(const TIP&) = delete; // No copy ctro.
        /// Assignment operator disabled.
        TIP& operator=(const TIP&) = delete;

        //== Error control methods
        /// Returns the parsing operation status.
        bool parsing_ok(void) const{
            return m_error_code==code_t::OK;
        };
        /// Returns the current error message.
        string parser_error_msg(void) const;

        /// Retrieve the list of all fields for a given section name.
        vector<string> get_fields_list(c_str_ref section_name) const;
        /// Retrieve the list of all sections found in this file.
        vector<string> get_sections_list(void) const;
        /// Returns a string with a pretty printed version of the database content.
        string pretty_print(void) const;

        //== Field access methods
        /// Retrieves a string from the database.
        string get_str  (c_str_ref section, c_str_ref field, c_str_ref fbv="empty");
        /// Retrieves an integer value from the database.
        int    get_int  (c_str_ref section, c_str_ref field, int       fbv=0);
        /// Retrieves a bool value from the database.
        bool   get_bool (c_str_ref section, c_str_ref field, bool      fbv=false);
        /// Retrieves a real value from the database.
        real_t get_real (c_str_ref section, c_str_ref field, real_t    fbv=0.f);
};

#endif /* end of include guard: TIP_H */
