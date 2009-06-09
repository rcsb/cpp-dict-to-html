/*$$FILE$$*/
/*$$VERSION$$*/
/*$$DATE$$*/
/*$$LICENSE$$*/


#include <string>

/*
 *   These flags are used by the navigation function... 
 */
#define F_TOP_MENU               0x000001
#define F_DICT_INFO              0x000002
#define F_CATEGORY_GROUP_MENU    0x000004
#define F_CATEGORY_MENU          0x000008
#define F_ITEM_MENU              0x000010
#define F_DATA_MENU              0x000020


/*
 *  Colors, font sizes and icons are selected according to viewer type..
 * 
 */

#define    HT_BG_COLOR           "#ffffff"

#define    HT_IMAGE_BAR          "bar-bluepurple.gif"
#define    HT_ICON_DEFAULT       "redball.gif"
#define    HT_ICON_DEFAULT1      "blueball.gif"

#define    HT_ICON_ALIAS         "ALIAS.gif"
#define    HT_ICON_CATEGORIES    "CATEGORIES.gif"
#define    HT_ICON_CATEGORY      "CATEGORY.gif"
#define    HT_ICON_CHILD         "CHILD.gif"
#define    HT_ICON_DATA          "DATA.gif"
#define    HT_ICON_DATATYPE      "DATATYPE.gif"
#define    HT_ICON_VDEFAULT      "DEFAULT.gif"
#define    HT_ICON_DEPENDENCY    "DEPENDENCY.gif"
#define    HT_ICON_DESCRIPTION   "DESCRIPTION.gif"
#define    HT_ICON_ENUMERATION   "ENUMERATION.gif"
#define    HT_ICON_EXAMPLE       "EXAMPLE.gif"
#define    HT_ICON_EXAMPLES      "EXAMPLES.gif"
#define    HT_ICON_GROUPS        "GROUPS.gif"
#define    HT_ICON_GROUP         "GROUP.gif"
#define    HT_ICON_ITEMS         "ITEMS.gif"
#define    HT_ICON_ITEM          "ITEM.gif"
#define    HT_ICON_KEY           "KEY.gif"
#define    HT_ICON_KEYS          "KEYS.gif"
#define    HT_ICON_MAN           "MAN.gif"
#define    HT_ICON_MIS           "MIS.gif"
#define    HT_ICON_PARENT        "PARENT.gif"
#define    HT_ICON_RANGE         "RANGE.gif"
#define    HT_ICON_RELATED       "RELATED.gif"
#define    HT_ICON_SUBCATEGORY   "SUBCATEGORY.gif"
#define    HT_ICON_TYPE          "TYPE.gif"
#define    HT_ICON_UNITS         "UNITS.gif"

#define    HT_BASE_FONT_SIZE            4
#define    HT_MENU_NAME_FONT_SIZE       6
#define    HT_MENU_ITEM_FONT_SIZE       5
#define    HT_LIST_ITEM_FONT_SIZE       4
#define    HT_TABLE_HEADER_FONT_SIZE    4
#define    HT_TABLE_ITEM_FONT_SIZE      4
#define    HT_NAVIGATE_FONT_SIZE        4
#define    HT_LABEL_FONT_SIZE           5

#define LF 10
#define CR 13


#define HT_MAX_LINE_LENGTH      1000
#define HT_MAX_URL_LENGTH       256



/*
 *  Prototypes ... 
 *
 */


void htput_title(FILE *fp,const char *title);
void htput_open_body(FILE *fp, const char *title);
void htput_close_body(FILE *fp);
void htput_separator(FILE *fp);
void htput_ndb_address(FILE *fp);
void htput_header1_text(FILE *fp,const char *text);
void htput_header1_text_with_icon(FILE *fp, const char *text,  const char *icon);
void htput_header2_text(FILE *fp,const char *text);
void htput_header2_text_with_icon(FILE *fp, const char *text,  const char *icon);
void htput_header3_text(FILE *fp,const char *text);
void htput_header3_text_with_icon(FILE *fp, const char *text,  const char *icon);
void htput_paragraph_break(FILE *fp);
void htput_line_break(FILE *fp);
void htput_text(FILE *fp,const char *text);
void htput_centered_text(FILE *fp,const char *text);
void htput_centered_menu_text(FILE *fp,const char *name);
void htput_centered_menu_file_item(FILE *fp,const char *file, const char *item);
void htput_open_definition_list(FILE *fp);
void htput_def_item1(FILE *fp, const char *subpath, const char *file, const char *item, const char *def);
void htput_close_definition_list(FILE *fp);
void htput_open_unordered_list(FILE *fp);
void htput_list_item1(FILE *fp, const char *subpath, const char *file, const char *item);
void htput_close_unordered_list(FILE *fp);
void htput_pull_down_selection(FILE *fp, const char *label, const char *tag, char **items, int nitems);
void htput_scrolled_selection(FILE *fp,const char *label, const char *tag, char **items, 
			      int nitems, int ndisplay);
void htput_selection_navigator(FILE *fp, long flags);
void htput_open_center(FILE *fp);
void htput_close_center(FILE *fp);
void htput_formatted_section(FILE *fp, const char *section, const char *text);
void htput_formatted_section_with_icon(FILE *fp, const char *section, const char *text, const char *icon);
void htput_example_section(FILE *fp, const std::string& description, const char *text, int numb);
void htput_example_section_with_icon(FILE *fp, const std::string& description, const char *text, int numb, const char *icon);
void htput_open_preformat(FILE *fp);
void htput_close_preformat(FILE *fp);
void ht_strip_nl(char *string);

void htput_open_table(FILE *fp);
void htput_open_table_with_border(FILE *fp);
void htput_close_table(FILE *fp);
void htput_open_row(FILE *fp);
void htput_close_row(FILE *fp);
void htput_table_col_text(FILE *fp, const char *text);
void htput_table_col_text_left(FILE *fp, const char *text);
void htput_table_col_formatted_text(FILE *fp, const char *text);
void htput_table_col_url(FILE *fp, const char *subpath, const char *file, const char *item);
void htput_table_col_header(FILE *fp, const char *text);
void htput_labeled_ref1(FILE *fp, const char *subpath, const char *file, const char *label, const char *item);
void htput_labeled_ref1_with_icon(FILE *fp, const char *subpath, const char *file, const char *label, const char *item, const char *icon);
void htput_labeled_text(FILE *fp, const char *label, const char *text);
void htput_labeled_text_with_icon(FILE *fp, const char *label, const char *text, const char *icon);
void htput_label_with_icon(FILE *fp, const char *label,  const char *icon);

void htput_centered_label(FILE *fp,const char *label);


