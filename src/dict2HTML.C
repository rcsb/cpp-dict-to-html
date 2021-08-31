/*$$FILE$$*/
/*$$VERSION$$*/
/*$$DATE$$*/
/*$$LICENSE$$*/
/*
 *
 *  This example demonstrates the construction of an HTML tree for the 
 *      Macromolecular CIF Dictionary using access functions of the 
 *            CIFOBJ C++ Application Program Interface ....
 *
 *        dict2HTML -dictSdbFile <dictionary SDB file>  
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>

#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "GenString.h"
#include "htmlUtil.h"
#include "DicFile.h"

using std::string;
using std::vector;
using std::map;
using std::sort;
using std::cerr;
using std::endl;

#define MAXROW   128
#define MAXLINE  8192

int igbuf[MAXLINE];
// cbuf1 gets potentially MAXLINE + extra strings
char cbuf1[MAXLINE*2], cbuf2[MAXLINE], filebuffer[MAXROW];
char fbuf1[MAXLINE];

/* char *blank = "                  "; */
const char* blank = "n.a.";

char *DICTIONARY_VERSION;
char *HTML_PATH;

int demoGetArgs(int argc, char *argv[],DicFile ** dic, string &dicFileName);
int   selectionQuery1(DicFile * dic, const char *dataBlockName,
                      const char *categoryName,
                      const char *keyword, 
                      const char *value,
							 vector<unsigned int>& listOut);
void getDictInfo(DicFile * dic, const string &htmlPath);
void getCategoryGroupInfo(DicFile * dic);
void getCategoryInfo(DicFile * dic);
void getItemInfo(DicFile * dic);
void escapeFileName(const char* istr, char fn[MAXLINE]);

void escapeFileName(const char* istr, char fn[MAXLINE]) 
{
  int i;
  
  strcpy(fn,"");
  if ((istr == NULL) || (strlen(istr) == 0) ) return;
  for (i=0; i < (int) strlen(istr); i++) {
    if (istr[i] == '/') {
      fn[i] = '_';
    }else{
      fn[i] = istr[i];
    }
  }
  fn[i] = '\0';
}


int   selectionQuery1(DicFile * dic, const char *dataBlockName,
		      const char *categoryName,
		      const char *keyword, 
		      const char *value,
				vector<unsigned int>& listOut)
{
  int nhits;
  ISTable * tbl;

/* 
 *  Reset the query 
 */
  nhits = 0;
  if (value == NULL) return(0);
/*
 *  Convert names to indices ...
 */
  Block& block = dic->GetBlock(dataBlockName);
  tbl = block.GetTablePtr(categoryName);
  if (tbl == NULL) return(0);
  if (!(tbl->IsColumnPresent(keyword))) return(0);
  
/*
 * Get the table length and fetch the array of indices 
 *          satisfying keyword == value
 */
  vector<string> target;
  vector<string> where;
  where.push_back(keyword);
  target.push_back(value);

  tbl->Search(listOut, target, where);
  if (!listOut.empty()) {
	 nhits = listOut.size();
	 return(nhits);
  }
  else
	 return(0);
}


void getDictInfo(DicFile * dic, const string &htmlPath)
/* 
 *  Make all required directory entries and make an index file for 
 *  the current dictionary version.  Assign the global value of
 *  DICTIONARY_VERSION...
 *
 */
{
  int j;

  vector<string> dataBlockNames;

  FILE *fp;
  long flags = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
  long flags1 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;

  string value , value1, value2 , value3, value4 ;
  int   nhits = 0;
  unsigned int   idb;
  vector<unsigned int> listOut;
  ISTable * tbl;

  idb = 1;

  dic->GetBlockNames(dataBlockNames);
  
  if (dataBlockNames.size() < idb ) exit(1);
/*
 *  Save the name of the dictionary and file name  ... 
 */
  HTML_PATH = (char *) calloc(strlen(htmlPath.c_str())+1,sizeof(char));
  strcpy(HTML_PATH,htmlPath.c_str());

  DICTIONARY_VERSION = (char *) calloc(strlen(dataBlockNames[0].c_str())+1,sizeof(char));
  strcpy(DICTIONARY_VERSION,dataBlockNames[0].c_str());

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s",HTML_PATH);
  cerr << "creating directory " << cbuf1 << endl;
  rmdir(cbuf1);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Groups",HTML_PATH);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Categories",HTML_PATH);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Items",HTML_PATH);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data",HTML_PATH);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Index",HTML_PATH);
  mkdir(cbuf1,0777);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Index/index.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"HTML Version of Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"HTML Version of Dictionary %s",DICTIONARY_VERSION);

  htput_header1_text(fp,cbuf1);

  htput_separator(fp);
  htput_open_center(fp);
  
/*
 *  Get dictionary information ....
 */
  nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			   "dictionary",
			   "datablock_id",
			   DICTIONARY_VERSION,listOut);
  if (nhits > 0) {
         Block& block = dic->GetBlock(dataBlockNames[0]);
	 tbl = block.GetTablePtr("dictionary");
    for (j = 0; j < nhits; j++) {
		value1 = (*tbl)(listOut[j], "title");
		value2 = (*tbl)(listOut[j], "version");
		htput_labeled_text(fp,"Dictionary Title",value1.c_str()); 
		htput_labeled_text(fp,"Dictionary Version",value2.c_str()); 
		htput_labeled_ref1(fp,"Data","history","Revision History","history_file"); 
		value1.clear();
		value2.clear();
	 }
  }
  htput_close_center(fp);
  htput_selection_navigator(fp,flags);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  
/********************************************************************
 *
 *  Write the index for the Data directory ... 
 */

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/index.html",HTML_PATH);
  fp = fopen(cbuf1,"w");
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Data Sections in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  
  htput_selection_navigator(fp,flags1);
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Data Sections in Dictionary %s",DICTIONARY_VERSION);
  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_open_unordered_list(fp); 
  htput_list_item1(fp,"Data","history","Dictionary Revision History");
  htput_list_item1(fp,"Data","subcategories","Subcategories");
  htput_list_item1(fp,"Data","types","Data Types");
  htput_list_item1(fp,"Data","units","Unit Descriptions");
  htput_list_item1(fp,"Data","conversions","Unit Conversions");
  htput_close_unordered_list(fp); 
  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  

/*********************************************************************
 *
 *  Dictionary history file ...
 */
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/history.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Revision History of Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  
  
  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  Block& block = dic->GetBlock(dataBlockNames[0]);
  tbl = block.GetTablePtr("dictionary_history");
  if (tbl != NULL ) {
	 nhits = tbl->GetNumRows();
    htput_line_break(fp);
    htput_line_break(fp);
    htput_open_table_with_border(fp);
    htput_open_row(fp);
    htput_table_col_header(fp,"Version");
    htput_table_col_header(fp,"Date");
    htput_table_col_header(fp,"Revision");
    htput_close_row(fp);

    for (j=nhits-1; j >= 0; j--) {
		value1.clear();
		value2.clear();
		value3.clear();
		value1 = (*tbl)(j, "version");
		value2 = (*tbl)(j, "update");
		value3 = (*tbl)(j, "revision");
      htput_open_row(fp);
      if (!CifString::IsEmptyValue(value1)) {
	htput_table_col_text(fp,value1.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value2)) {
	 htput_table_col_text(fp,value2.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value3)) {
	htput_table_col_formatted_text(fp,value3.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      htput_close_row(fp);
    }
    htput_close_table(fp);
  }

  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  

/*********************************************************************
 *
 *  Data Types file ...
 */
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/types.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Data Types in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  


  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  tbl = block.GetTablePtr("item_type_list");
  if (tbl != NULL ) {
	 nhits = tbl->GetNumRows();
    htput_line_break(fp);
    htput_line_break(fp);
    htput_open_table_with_border(fp);
    htput_open_row(fp);
    htput_table_col_header(fp,"Type Code");
    htput_table_col_header(fp,"Primitive Type");
    htput_table_col_header(fp,"Regular Expression");
    htput_table_col_header(fp,"Description");
    htput_close_row(fp);

    for (j=0; j < nhits; j++) {
		value1.clear();
		value2.clear();
		value3.clear();
		value4.clear();
		value1 = (*tbl)(j, "code");
		value2 = (*tbl)(j, "primitive_code");
		value3 = (*tbl)(j, "construct");
		value4 = (*tbl)(j, "detail");
      htput_open_row(fp);
      if (!CifString::IsEmptyValue(value1)) {
	htput_table_col_text(fp,value1.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value2)) {
	htput_table_col_text(fp,value2.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value3)) {
	htput_table_col_formatted_text(fp,value3.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value4)) {
	htput_table_col_formatted_text(fp,value4.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      htput_close_row(fp);
    }
    htput_close_table(fp);
  } else {
    
  }
  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  


/*********************************************************************
 *
 *  Units  file ...
 */
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/units.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Unit Descriptions in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  

  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  tbl = block.GetTablePtr("item_units_list");
  if (tbl !=NULL ) {
	 nhits = tbl->GetNumRows();
    htput_line_break(fp);
    htput_line_break(fp);
    htput_open_table_with_border(fp);
    htput_open_row(fp);
    htput_table_col_header(fp,"Units Code");
    htput_table_col_header(fp,"Description");
    htput_close_row(fp);

    for (j=0; j < nhits; j++) {
		value1.clear();
		value2.clear();
		value1 = (*tbl)(j, "code");
		value2 = (*tbl)(j, "detail");

      htput_open_row(fp);
      if (!CifString::IsEmptyValue(value1)) {
	htput_table_col_text(fp,value1.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value2)) {
	htput_table_col_formatted_text(fp,value2.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      htput_close_row(fp);
    }
    htput_close_table(fp);
  }
  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  

/*********************************************************************
 *
 *  Unit Conversions Types file ...
 */
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/conversions.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Unit Conversions in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  

  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  tbl = block.GetTablePtr("item_units_conversion");
  if (tbl != NULL ) {
	 nhits = tbl->GetNumRows();
    htput_line_break(fp);
    htput_line_break(fp);
    htput_open_table_with_border(fp);
    htput_open_row(fp);
    htput_table_col_header(fp,"Convert From");
    htput_table_col_header(fp,"Convert to");
    htput_table_col_header(fp,"Operator");
    htput_table_col_header(fp,"Factor");
    htput_close_row(fp);

    for (j=0; j < nhits; j++) {
		value1.clear();
		value2.clear();
		value3.clear();
		value4.clear();
		value1 = (*tbl)(j, "from_code");
		value2 = (*tbl)(j, "to_code");
		value3 = (*tbl)(j, "operator");
		value4 = (*tbl)(j, "factor");
      htput_open_row(fp);
      if (!CifString::IsEmptyValue(value1)) {
	htput_table_col_text(fp,value1.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value2)) {
	htput_table_col_text(fp,value2.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value3)) {
	htput_table_col_text(fp,value3.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value4)) {
	htput_table_col_formatted_text(fp,value4.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      htput_close_row(fp);
    }
    htput_close_table(fp);
  }
  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  


/*********************************************************************
 *
 *  Subcategories file ...
 */
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Data/subcategories.html",HTML_PATH);
  fp = fopen(cbuf1,"w");

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Subcategories Defined in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);  

  htput_header1_text(fp,cbuf1);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  tbl = block.GetTablePtr("sub_category");
  if (tbl != NULL ) {
    nhits = tbl->GetNumRows();
    htput_line_break(fp);
    htput_line_break(fp);
    htput_open_table_with_border(fp);
    htput_open_row(fp);
    htput_table_col_header(fp,"Subcategory");
    htput_table_col_header(fp,"Description");
    htput_close_row(fp);

    for (j=0; j < nhits; j++) {
      value1.clear();
      value2.clear();
      value1 = (*tbl)(j, "id");
      value2 = (*tbl)(j, "description");
      
      htput_open_row(fp);
      if (!CifString::IsEmptyValue(value1)) {
	htput_table_col_text(fp,value1.c_str());
      } else {
	htput_table_col_text(fp,blank);
      }
      if (!CifString::IsEmptyValue(value2)) {
	htput_table_col_formatted_text(fp,value2.c_str());
      } else {
	htput_table_col_formatted_text(fp,blank);
      }
      htput_close_row(fp);
    }
    htput_close_table(fp);
  }
  htput_selection_navigator(fp,flags1);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);  
}


void getCategoryGroupInfo(DicFile * dic)
{
  StringLess ciLess(Char::eCASE_INSENSITIVE);
  map<string, string, StringLess> _groupMap(ciLess);
  map<string, string, StringLess>::iterator _it;
  string s1,s2;
  int i,j;

  vector<string> dataBlockNames; 

  string groupName;
  int    numGroup;

  vector<string> categoryNames;
  int    numCategory;

  string name, value, value1, value2;

  unsigned int   idb;
  ISTable * tbl;

  ISTable * tbl2;

  vector<string> target;
  vector<string> where;
  vector<unsigned int> listOut;

  FILE *fp, *fp1;
  long flags1 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
  long flags2 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
  dic->GetBlockNames(dataBlockNames);

/* 
 *              The target dictionary should be block index 1.
 */
  idb = 1;
  if (dataBlockNames.size() < idb ) {
    fprintf(stderr,"Unable to find target dictionary\n");
    exit(1);
  }
/*
 *    Get the list of category groups in the dictionary ...
 */

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Groups/index.html",HTML_PATH);
  fp = fopen(cbuf1,"w");
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Category Groups in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Category Groups in Dictionary %s",DICTIONARY_VERSION);
  htput_header1_text_with_icon(fp,cbuf1,HT_ICON_GROUPS);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);

  Block& block = dic->GetBlock(dataBlockNames[0]);
  tbl = block.GetTablePtr("category_group_list");
  if (tbl==NULL) return;
  numGroup = tbl->GetNumRows();
  if (numGroup > 0) {


    htput_open_definition_list(fp);
    for (i = 0; i < numGroup; i++) {
      groupName = (*tbl)(i, "id");
      if (groupName.size() != 0) {
      }
      s1=groupName.c_str();
      value = (*tbl)(i, "description");
      if (!CifString::IsEmptyValue(value)) { 
	s2=value.c_str();
      } else {
	s2="";
      }
      _groupMap[s1]=s2;
      value.clear();
    }
    
    for (_it = _groupMap.begin(); _it != _groupMap.end(); ++_it) {
      htput_def_item1(fp,"Groups",_it->first.c_str(),_it->first.c_str(),_it->second.c_str());
    }
    _groupMap.clear();
    htput_close_definition_list(fp);

    for (i = 0; i < numGroup; i++) {
      
      /*
       *   Get the category group name ...
       */
      groupName = (*tbl)(i, "id");
      if (groupName.size() != 0) {
      }
      /*
       *   Get the category group description ...
       */
      
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"./%s/Groups/%s.html",HTML_PATH,groupName.c_str());
      fp1 = fopen(cbuf1,"w");
      memset(cbuf1,'\0',MAXLINE); 
      sprintf(cbuf1,"Cateogies in Group %s",groupName.c_str());
      htput_open_body(fp1,cbuf1);
      
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Categories in Group <i>%s</i>",groupName.c_str());
      htput_header1_text_with_icon(fp1,cbuf1,HT_ICON_GROUP);
      htput_separator(fp1);
      htput_selection_navigator(fp1,flags1);

      Block& block = dic->GetBlock(dataBlockNames[0]);
      tbl2 = block.GetTablePtr("category_group");
      target.clear();
      where.clear();
      target.push_back(groupName);
      where.push_back("id");
      tbl2->Search(listOut, target, where);
      if (!listOut.empty()) 
	numCategory = listOut.size();
      else
	numCategory = 0;
      //ikey = tbl2->GetColumnIndex("category_id");
      if (numCategory > 0 ) {
	for (j = 0; j < numCategory; j++) {
	  value = (*tbl2)(listOut[j], "category_id");
	  categoryNames.push_back(value);
	}
	sort(categoryNames.begin(), categoryNames.end());
	
      /*	 
       *    Write the url list for each category in this group
       */	
	htput_open_unordered_list(fp1);
	for (j = 0; j < numCategory; j++) {
	  htput_list_item1(fp1,"Categories",categoryNames[j].c_str(),categoryNames[j].c_str());
	  categoryNames[j].clear();
	}
	htput_close_unordered_list(fp1);
	categoryNames.clear();		
      }
      htput_selection_navigator(fp1,flags1);
      htput_ndb_address(fp1);
      htput_close_body(fp1);
      fclose(fp1);
      


    } /* end of group loop */

  }
  htput_selection_navigator(fp,flags2);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);
}


void getCategoryInfo(DicFile * dic)
{
  StringLess ciLess(Char::eCASE_INSENSITIVE);
  map<string, string, StringLess> _categoryMap(ciLess);
  map<string, string, StringLess> _itemMap(ciLess);
  map<string, string, StringLess>::iterator _it;
  string s1,s2;

  int i,j;

  vector<string> dataBlockNames;



  int    numCategory;

  string categoryName;
  string name, value, value1, value2;
  int   nhits = 0;
  unsigned int   idb;
  ISTable *tbl, *tblNdbD, *tblEx ,*tblNdbEx ,*tblKey , *tblItem,  *tblPdbxD, *tblPdbxEx;
  vector<unsigned int> listOut;

  FILE *fp, *fp1;
  long flags1 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
  long flags2 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
 
  dic->GetBlockNames(dataBlockNames);

/* 
 *              The target dictionary should be block index 1.
 */
  idb = 1;
  if (dataBlockNames.size() < idb ) {
    fprintf(stderr,"Unable to find target dictionary\n");
    exit(1);
  }


/*
 *  For each category print the ....
 */

  Block& block1 = dic->GetBlock(dataBlockNames[idb-1]);
  tbl = block1.GetTablePtr("category");
  if (tbl == NULL) return;
  //ikey = tbl->GetColumnIndex("description");
  //idkey = tbl->GetColumnIndex("id");
  numCategory = tbl->GetNumRows();
  
  tblNdbD = block1.GetTablePtr("ndb_category_description");
  tblPdbxD = block1.GetTablePtr("pdbx_category_description");
  if (tblNdbD != NULL)  //ikeyNdbD = tblNdbD->GetColumnIndex("description")
    ;

  Block& block = dic->GetBlock(dataBlockNames[0]);
  tblEx = block.GetTablePtr("category_examples");
  if (tblEx != NULL) {
	 //ikeyEx1 = tblEx->GetColumnIndex("case");
  }
  tblNdbEx = block.GetTablePtr("ndb_category_examples");
  tblPdbxEx = block.GetTablePtr("pdbx_category_examples");
  if (tblNdbEx != NULL) {
	 //ikeyNdbEx1 = tblNdbEx->GetColumnIndex("case");
  }


  tblKey = block.GetTablePtr("category_key");
  if (tblKey != NULL) {
	 //ikeyKey1 = tblKey->GetColumnIndex("name");
  }
  tblItem = block.GetTablePtr("item");
  if (tblItem != NULL) {
	 // ikeyItem1 = tblItem->GetColumnIndex("name");
	 // ikeyItem2 = tblItem->GetColumnIndex("mandatory_code");
  }

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Categories/index.html",HTML_PATH);
  fp = fopen(cbuf1,"w");
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Cateogy Index for Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Categories in Dictionary %s",DICTIONARY_VERSION);
  htput_header1_text_with_icon(fp,cbuf1,HT_ICON_CATEGORIES);

  htput_separator(fp);
  htput_selection_navigator(fp,flags1);
  htput_open_unordered_list(fp);

  for (i=0; i < numCategory; i++) {
    categoryName = (*tbl)(i, "id");
    s1=categoryName.c_str();

    escapeFileName(categoryName.c_str(),fbuf1);  
    s2=fbuf1;
    _categoryMap[s1]=s2;
    categoryName.clear();
  }
  for (_it = _categoryMap.begin(); _it != _categoryMap.end(); ++_it) {
    htput_list_item1(fp,"Categories",_it->second.c_str(),_it->first.c_str());
  }
  _categoryMap.clear();

  htput_close_unordered_list(fp);
  htput_selection_navigator(fp,flags2);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);

  for (i=0; i < numCategory; i++) {
/*
 *   Get the category name ...
 */
    categoryName = (*tbl)(i, "id");
    escapeFileName(categoryName.c_str(),fbuf1);  
    //    htput_list_item1(fp,"Categories",fbuf1,categoryName.c_str());

    memset(cbuf1,'\0',MAXLINE);  
    sprintf(cbuf1,"./%s/Categories/%s.html",HTML_PATH,fbuf1);
    fp1 = fopen(cbuf1,"w");

    memset(cbuf1,'\0',MAXLINE); 
    sprintf(cbuf1,"Definition of Category %s",categoryName.c_str());
    htput_open_body(fp1,cbuf1);
    
    memset(cbuf1,'\0',MAXLINE);  
    sprintf(cbuf1,"Category  <i>%s</i>",categoryName.c_str());
    htput_header1_text_with_icon(fp1,cbuf1,HT_ICON_CATEGORY);
    htput_separator(fp1);
    htput_selection_navigator(fp1,flags1);
    htput_open_unordered_list(fp1);

/*
 *   Get the category description ...
 */
    value = (*tbl)(i, "description");
    if (!CifString::IsEmptyValue(value)) {  
      htput_formatted_section_with_icon(fp1,"Category Description", value.c_str(), HT_ICON_DESCRIPTION);
    }

    nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			    "ndb_category_description",
			    "id",
			    categoryName.c_str(),
				  listOut);
    if (nhits > 0) {
      for (j = 0; j < nhits; j++) {
	value = (*tblNdbD)(listOut[j], "description");
	if (!CifString::IsEmptyValue(value)) {  
	  htput_formatted_section_with_icon(fp1,"Local Description", value.c_str(),HT_ICON_DESCRIPTION);
	  value.clear(); 
	}
      }
    }
    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			    "pdbx_category_description",
			    "id",
			    categoryName.c_str(),
			     listOut);
    if (nhits > 0) {
      for (j = 0; j < nhits; j++) {
	value = (*tblPdbxD)(listOut[j], "description");
	if (!CifString::IsEmptyValue(value)) {  
	  htput_formatted_section_with_icon(fp1,"Deposition Description", value.c_str(),HT_ICON_DESCRIPTION);
	  value.clear(); 
	}
      }
    }
    listOut.clear();


/*
 *  Get the category examples ...
 */
    nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			    "category_examples",
			    "id",
			    categoryName.c_str(), listOut);
    value2.clear(); 
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Category Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tblEx)(listOut[j], "case");
                  if (tblEx->IsColumnPresent("detail"))
		    value2 = (*tblEx)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) {  
	  htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
	  value1.clear(); 
	  value2.clear(); 
	}
      } 
    };
    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			    "ndb_category_examples",
			    "id",
			    categoryName.c_str(), listOut);
    value2.clear(); 
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Local Category Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tblNdbEx)(listOut[j], "case");
                  if (tblNdbEx->IsColumnPresent("detail"))
		    value2 = (*tblNdbEx)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) {  
	  htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
	  value1.clear(); 
	  value2.clear(); 
	}
      } 
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[0].c_str(),
			    "pdbx_category_examples",
			    "id",
			    categoryName.c_str(), listOut);
    value2.clear(); 
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Deposition Category Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tblPdbxEx)(listOut[j], "case");
                  if (tblPdbxEx->IsColumnPresent("detail"))
		    value2 = (*tblPdbxEx)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) {  
	  htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
	  value1.clear(); 
	  value2.clear(); 
	}
      } 
    }
    listOut.clear();

/*
 *  Get the category key items ...
 */
    nhits  = selectionQuery1(dic, dataBlockNames[0].c_str(),
			    "category_key",
			    "id",
			    categoryName.c_str(), listOut);
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Key Category Items",HT_ICON_KEYS);
      htput_open_unordered_list(fp1);
      for (j = 0; j < nhits; j++) {
	value1 = (*tblKey)(listOut[j], "name");
	if (!CifString::IsEmptyValue(value1)) {  
	  escapeFileName(value1.c_str(), fbuf1);
          htput_list_item1(fp1,"Items",fbuf1,value1.c_str());
	  value1.clear();
	}
      }
      htput_close_unordered_list(fp1);
    }
	 listOut.clear();
/*
 *  Get the list of items in the category
 */
    nhits = selectionQuery1(dic, dataBlockNames[idb-1].c_str(),
			    "item",
			    "category_id",
			    categoryName.c_str(), listOut);
    
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Items in Category <i>%s</i>",categoryName.c_str());
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_ITEMS);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_table_col_header(fp1,"Item Name");
      htput_table_col_header(fp1,"Mandatory Code");


      for (j = 0; j < nhits; j++) {
	value1 = (*tblItem)(listOut[j], "name");
	value2 = (*tblItem)(listOut[j], "mandatory_code");
	if (!CifString::IsEmptyValue(value1)) {  
	  s1=value1.c_str();
	  if (!CifString::IsEmptyValue(value2)) {
	    s2=value2.c_str();
	    value2.clear(); 
	  } else {
	    s2 = "";
	  }
	  _itemMap[s1] = s2;
	  value1.clear();
	}


      }
      for (_it = _itemMap.begin(); _it != _itemMap.end(); ++_it) {
	htput_open_row(fp1);
	escapeFileName(_it->first.c_str(),fbuf1);
	htput_table_col_url(fp1, "Items", fbuf1, _it->first.c_str());
	if (!CifString::IsEmptyValue(_it->second)) {  
	  htput_table_col_text(fp1,_it->second.c_str());
	}
	htput_close_row(fp1);
	value1.clear();
	value2.clear(); 
      }
      _itemMap.clear();

      htput_close_table(fp1);
      htput_open_center(fp1);
  } 
  
    htput_selection_navigator(fp1,flags1);
    htput_ndb_address(fp1);
    htput_close_body(fp1);
    fclose(fp1);
    categoryName.clear();
  } /* end of category loop */
  
  
}


void getItemInfo(DicFile * dic)
{
  StringLess ciLess(Char::eCASE_INSENSITIVE);
  map<string, string, StringLess> _itemMap(ciLess);
  map<string, string, StringLess>::iterator _it;
  string s1,s2;

  int i,j;

  vector<string> dataBlockNames;
 
  string itemName;
  int  numItem;

  string value , value1, value2, value3;
  int   nhits = 0;
  unsigned int   idb;
  ISTable *tblitem;
  ISTable *tbl;
  vector<unsigned int> listOut;


  FILE *fp, *fp1;
  long flags1 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;
  long flags2 = F_TOP_MENU | F_DICT_INFO | F_CATEGORY_GROUP_MENU | F_CATEGORY_MENU | F_ITEM_MENU | F_DATA_MENU;


  dic->GetBlockNames(dataBlockNames);

/* 
 *              The target dictionary should be block index 1.
 */
  idb = 1;
  if (dataBlockNames.size() < idb ) {
    fprintf(stderr,"Unable to find target dictionary\n");
    exit(1);
  }

/*
 *  For each item print the ....
 */

  Block& block1 = dic->GetBlock(dataBlockNames[idb-1]);
  tblitem = block1.GetTablePtr("item");
  if (tblitem == NULL) return;
  //idkey = tblitem->GetColumnIndex("name");
  //imkey = tblitem->GetColumnIndex("mandatory_code");
  //ickey = tblitem->GetColumnIndex("category_id");
  numItem = tblitem->GetNumRows();
  if (numItem <= 0 ) return;

  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"./%s/Items/index.html",HTML_PATH);
  fp = fopen(cbuf1,"w");
  memset(cbuf1,'\0',MAXLINE); 
  sprintf(cbuf1,"Items in Dictionary %s",DICTIONARY_VERSION);
  htput_open_body(fp,cbuf1);
  htput_header1_text_with_icon(fp,cbuf1,HT_ICON_ITEMS);
  htput_separator(fp);
  htput_selection_navigator(fp,flags1);
  htput_open_unordered_list(fp);


  for (i=0; i < numItem; i++) {
    itemName = (*tblitem)(i, "name");
    s1=itemName.c_str();
    escapeFileName(itemName.c_str(),fbuf1);
    s2=fbuf1;
    _itemMap[s1]=s2;
    itemName.clear();
  }
  for (_it = _itemMap.begin(); _it != _itemMap.end(); ++_it) {
    htput_list_item1(fp,"Items",_it->second.c_str(),_it->first.c_str());
  }
  _itemMap.clear();
  htput_close_unordered_list(fp);
  htput_selection_navigator(fp,flags2);
  htput_ndb_address(fp);
  htput_close_body(fp);
  fclose(fp);

  for (i=0; i < numItem; i++) {

/*
 *   Get the item name ...
 */
    itemName = (*tblitem)(i, "name");
    escapeFileName(itemName.c_str(),fbuf1);
    strcpy(filebuffer,itemName.c_str()); 
    //    htput_list_item1(fp,"Items",fbuf1,itemName.c_str());
      
    memset(cbuf1,'\0',MAXLINE);  

    sprintf(cbuf1,"./%s/Items/%s.html",HTML_PATH,fbuf1);
    fp1 = fopen(cbuf1,"w");
    memset(cbuf1,'\0',MAXLINE); 
    sprintf(cbuf1,"Definition of Item %s",itemName.c_str());
    htput_open_body(fp1,cbuf1);
    
    memset(cbuf1,'\0',MAXLINE);  
    sprintf(cbuf1,"Item  <i>%s</i>",itemName.c_str());
    htput_header1_text_with_icon(fp1,cbuf1,HT_ICON_ITEM);
    htput_separator(fp1);
    htput_selection_navigator(fp1,flags1);

/************************************************************************
 *  Get the item description  ....
 */
    nhits  = selectionQuery1(dic, dataBlockNames[idb-1].c_str(),
			    "item_description",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("item_description");
		//ikey = tbl->GetColumnIndex("description");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "description");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_formatted_section_with_icon(fp1,"Description", value1.c_str(),HT_ICON_DESCRIPTION);
	  value1.clear();
		  }
      }
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "ndb_item_description",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("ndb_item_description");
		//ikey = tbl->GetColumnIndex("description");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "description");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_formatted_section_with_icon(fp1,"Local Description", value1.c_str(),HT_ICON_DESCRIPTION);
	  value1.clear();
	}
      }
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			     "pdbx_item_description",
			     "name",
			     itemName.c_str(),listOut);
    if (nhits > 0) {
      tbl = block1.GetTablePtr("pdbx_item_description");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "description");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_formatted_section_with_icon(fp1,"Deposition Description", value1.c_str(),HT_ICON_DESCRIPTION);
	  value1.clear();
	}
      }
    }

    listOut.clear();

/************************************************************************
 *   Get the Category id...
 */
    value = (*tblitem)(i, "category_id");
    if (value.c_str() != 0)    { 
      escapeFileName(value.c_str(),fbuf1);
      htput_labeled_ref1_with_icon(fp1,"Categories",fbuf1,"Category",value.c_str(),HT_ICON_CATEGORY); 
     value.clear();
    }

/************************************************************************
 *  Get the item examples ...
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_examples",
			    "name",
			    itemName.c_str(),listOut);
    value2.clear();
    if (nhits > 0) {
		tbl = block1.GetTablePtr("item_examples");

      htput_header2_text_with_icon(fp1,"Item Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "case");
                  if (tbl->IsColumnPresent("detail"))
		      value2 = (*tbl)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
			 value1.clear();
			 value2.clear();
		  }
		}
    }
	 
    listOut.clear();
		  
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "ndb_item_examples",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("ndb_item_examples");
      htput_header2_text_with_icon(fp1,"Local Item Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "case");
		  value2 = (*tbl)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
			 value1.clear();
			 value2.clear();
		  }
      }
    }

    listOut.clear();
    //
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "pdbx_item_examples",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("pdbx_item_examples");
      htput_header2_text_with_icon(fp1,"Deposition Item Examples",HT_ICON_EXAMPLES);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "case");
		  value2 = (*tbl)(listOut[j], "detail");
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_example_section_with_icon(fp1,value2,value1.c_str(),j+1,HT_ICON_DEFAULT1);
			 value1.clear();
			 value2.clear();
		  }
      }
    }
    listOut.clear();


/************************************************************************
 *   Get the mandatory code ...
 */
    value = (*tblitem)(i, "mandatory_code");
    if (!CifString::IsEmptyValue(value)) { 
      htput_labeled_text_with_icon(fp1,"Mandatory Code",value.c_str(),HT_ICON_MAN); 
			 value.clear();
    }


    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			     "ndb_item",
			     "name",
			     itemName.c_str(),listOut);
    if (nhits > 0) {
      tbl = block1.GetTablePtr("ndb_item");
      for (j = 0; j < nhits; j++) {
	value1 = (*tbl)(listOut[j], "mandatory_code");
	if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_text_with_icon(fp1,"Local Mandatory Code",value1.c_str(),HT_ICON_MAN); 
	  value1.clear();
	}
      }
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			     "pdbx_item",
			     "name",
			     itemName.c_str(),listOut);
    if (nhits > 0) {
      tbl = block1.GetTablePtr("pdbx_item");
      for (j = 0; j < nhits; j++) {
	value1 = (*tbl)(listOut[j], "mandatory_code");
	if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_text_with_icon(fp1,"Deposition Mandatory Code",value1.c_str(),HT_ICON_MAN); 
	  value1.clear();
	}
      }
    }

    listOut.clear();


/************************************************************************
 *  Get the item type 
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_type",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("item_type");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "code");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_ref1_with_icon(fp1,"Data","types","Data Type Code",value1.c_str(),HT_ICON_TYPE);
			 value1.clear(); 
	}
      }
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "ndb_item_type",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
      tbl = block1.GetTablePtr("ndb_item_type");
      for (j = 0; j < nhits; j++) {
	value1 = (*tbl)(listOut[j], "code");
	if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_ref1_with_icon(fp1,"Data","types","Local Data Type Code",value1.c_str(),HT_ICON_TYPE);
	  value1.clear(); 
	}
      }
    }

    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "pdbx_item_type",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
      tbl = block1.GetTablePtr("pdbx_item_type");
      for (j = 0; j < nhits; j++) {
	value1 = (*tbl)(listOut[j], "code");
	if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_ref1_with_icon(fp1,"Data","types","Deposition Data Type Code",value1.c_str(),HT_ICON_TYPE);
	  value1.clear(); 
	}
      }
    }

    listOut.clear();


/************************************************************************
 *  Get the item units
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_units",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("item_units");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "code");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_ref1_with_icon(fp1,"Data","units","Units",value1.c_str(),HT_ICON_UNITS); 
			 value1.clear();  
	}
      }
    }

	 listOut.clear();
/************************************************************************
 *  Get the item default value
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_default",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
		tbl = block1.GetTablePtr("item_default");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "value");
		  if (!CifString::IsEmptyValue(value1)) { 
	  htput_labeled_text_with_icon(fp1,"Default Value",value1.c_str(),HT_ICON_VDEFAULT); 
			 value1.clear(); 
		  }
      }
    }
	 listOut.clear();

/************************************************************************
 *  Get the item enumeration ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_enumeration",
			    "name",
				 itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Enumeration Values");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_ENUMERATION);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Item Value");
      htput_table_col_header(fp1,"Description");
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_enumeration");
      htput_close_row(fp1);
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "value");
		  value2 = (*tbl)(listOut[j], "detail");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }

    listOut.clear();
    //
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "ndb_item_enumeration",
			    "name",
			    itemName.c_str(), listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Local Enumeration Values");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_ENUMERATION);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Item Value");
      htput_table_col_header(fp1,"Description");
      htput_close_row(fp1);
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("ndb_item_enumeration");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "value");
		  value2 = (*tbl)(listOut[j], "detail");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }
    listOut.clear();

    //
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			     "pdbx_item_enumeration",
			     "name",
			     itemName.c_str(), listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Deposition Enumeration Values");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_ENUMERATION);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Item Value");
      htput_table_col_header(fp1,"Description");
      htput_close_row(fp1);
      Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
      tbl = currBlock.GetTablePtr("pdbx_item_enumeration");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "value");
		  value2 = (*tbl)(listOut[j], "detail");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }
    listOut.clear();



/************************************************************************
 *  Get the item range  ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_range",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Enumeration Range");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_RANGE);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Minimum Value");
      htput_table_col_header(fp1,"Maximum Value");
      htput_close_row(fp1);

      Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
      tbl = currBlock.GetTablePtr("item_range");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "minimum");
		  value2 = (*tbl)(listOut[j], "maximum");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
		  
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }
    listOut.clear();

    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "ndb_item_range",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Local Enumeration Range");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_RANGE);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Minimum Value");
      htput_table_col_header(fp1,"Maximum Value");
      htput_close_row(fp1);

      Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
      tbl = currBlock.GetTablePtr("ndb_item_range");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "minimum");
		  value2 = (*tbl)(listOut[j], "maximum");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
		  
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }
    listOut.clear();


    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "pdbx_item_range",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Deposition Enumeration Range");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_RANGE);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Minimum Value");
      htput_table_col_header(fp1,"Maximum Value");
      htput_close_row(fp1);

      Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
      tbl = currBlock.GetTablePtr("pdbx_item_range");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "minimum");
		  value2 = (*tbl)(listOut[j], "maximum");
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) { 
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) { 
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
		  
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }
    listOut.clear();



/************************************************************************
 *  Get the item aliases ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_aliases",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Alias Names");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_ALIAS);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Alias Name");
      htput_table_col_header(fp1,"Dictionary");
      htput_table_col_header(fp1,"Version");
      htput_close_row(fp1);

      Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
      tbl = currBlock.GetTablePtr("item_aliases");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "alias_name");
		  value2 = (*tbl)(listOut[j], "dictionary");
		  value3 = (*tbl)(listOut[j], "version");
		  htput_open_row(fp1); 
		  if (!CifString::IsEmptyValue(value1)) {
			 htput_table_col_text(fp1,value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) {
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value3)) {
			 htput_table_col_text(fp1,value3.c_str());
			 value3.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }

	 listOut.clear();
/************************************************************************
 *  Get the dependent items
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_dependent",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Dependent Items",HT_ICON_DEPENDENCY);
      htput_open_unordered_list(fp1);
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_dependent");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "dependent_name");
		  if (!CifString::IsEmptyValue(value1)) {
			 escapeFileName(value1.c_str(),fbuf1);
			 htput_list_item1(fp1,"Items",fbuf1,value1.c_str());
			 value1.clear(); 
		  }
      }
      htput_close_unordered_list(fp1);
    }


	 listOut.clear();
/************************************************************************
 *  Get the parent items ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_linked",
			    "child_name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Parent Items",HT_ICON_PARENT);
      htput_open_unordered_list(fp1);
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_linked");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "parent_name");
		  if (!CifString::IsEmptyValue(value1)) {
			 escapeFileName(value1.c_str(),fbuf1);
			 htput_list_item1(fp1,"Items",fbuf1,value1.c_str());
			 value1.clear(); 
		  }
      }
      htput_close_unordered_list(fp1);
    }

	 listOut.clear();
/************************************************************************
 *  Get the child items ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_linked",
			    "parent_name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Child Items",HT_ICON_CHILD);
      htput_open_unordered_list(fp1);
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_linked");
      for (j = 0; j < nhits; j++) {
	value1 = (*tbl)(listOut[j], "child_name");
	if (!CifString::IsEmptyValue(value1)) {
	  escapeFileName(value1.c_str(),fbuf1);
          htput_list_item1(fp1,"Items",fbuf1,value1.c_str());
	  value1.clear();
	}
      }
      htput_close_unordered_list(fp1);
    }

	 listOut.clear();
/************************************************************************
 *  Get the related items   ....
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_related",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      memset(cbuf1,'\0',MAXLINE);  
      sprintf(cbuf1,"Related Items");
      htput_header2_text_with_icon(fp1,cbuf1,HT_ICON_RELATED);
      htput_open_center(fp1);
      htput_open_table_with_border(fp1);
      htput_open_row(fp1);
      htput_table_col_header(fp1,"Related Item");
      htput_table_col_header(fp1,"Related Function");
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_related");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "related_name");
		  value2 = (*tbl)(listOut[j], "function_code");
		  htput_close_row(fp1);
		  
		  htput_open_row(fp1);
		  if (!CifString::IsEmptyValue(value1)) {
			 escapeFileName(value1.c_str(),fbuf1);
			 htput_table_col_url(fp1, "Items", fbuf1, value1.c_str());
			 value1.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  if (!CifString::IsEmptyValue(value2)) {
			 htput_table_col_text(fp1,value2.c_str());
			 value2.clear();
		  } else {
			 htput_table_col_text(fp1,blank);
		  }
		  htput_close_row(fp1);
      }
      htput_close_table(fp1);
      htput_close_center(fp1);
    }

	 listOut.clear();
/************************************************************************
 *  Get the subcategory membership
 */
    nhits  = selectionQuery1(dic,dataBlockNames[idb-1].c_str(),
			    "item_sub_category",
			    "name",
			    itemName.c_str(),listOut);
    if (nhits > 0) {
      htput_header2_text_with_icon(fp1,"Subcategories",HT_ICON_SUBCATEGORY);
      htput_open_unordered_list(fp1);
                Block& currBlock = dic->GetBlock(dic->GetFirstBlockName());
		tbl = currBlock.GetTablePtr("item_sub_category");
      for (j = 0; j < nhits; j++) {
		  value1 = (*tbl)(listOut[j], "id");
		  if (!CifString::IsEmptyValue(value1)) {
          htput_list_item1(fp1,"Data","subcategories",value1.c_str());
	  value1.clear(); 
	}
      }
      htput_close_unordered_list(fp1);
    }

	 listOut.clear();

   /**************************************************/

    htput_selection_navigator(fp1,flags1);
    htput_ndb_address(fp1);
    htput_close_body(fp1);
    fclose(fp1);

	 itemName.clear();
  } /* end of item loop */

}


int main (int argc, char *argv[])
{
  int ret;
  DicFile *dic;
  string htmlPath;
  ret = demoGetArgs(argc, argv, &dic, htmlPath);
  if (ret==1) return(1);
  getDictInfo(dic,htmlPath);
  getCategoryGroupInfo(dic);
  getCategoryInfo(dic);;
  getItemInfo(dic);
  delete (dic);
}



