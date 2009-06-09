/*$$FILE$$*/
/*$$VERSION$$*/
/*$$DATE$$*/
/*$$LICENSE$$*/
/*
 *  Collection of utilities for generating mmcif -> html
 */ 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <string>

#include "htmlUtil.h"
#include "CifString.h"

using std::string;

static int  dq = '"';

/* 
 * Global string pointing to the current dictionary... 
 * Used to select the path 
 */

extern char *DICTIONARY_VERSION;

void htput_title(FILE *fp,const char *title)
{
  fprintf(fp,"<TITLE> %s </TITLE>\n",title);
}


void htput_open_body(FILE *fp, const char *title)
{
#if defined(HTMLSSI)
  fprintf(fp,"<!DOCTYPE HTML PUBLIC %c-//W3C//DTD HTML 4.01//EN%c>\n",dq,dq);
  fprintf(fp,"<HTML>\n");
  fprintf(fp,"<HEAD>\n");
  htput_title(fp,title);
  fprintf(fp,"<meta http-equiv=%cContent-Type%c content=%ctext/html; charset=iso-8859-1%c>\n",dq,dq,dq,dq);
  fprintf(fp,"<LINK HREF=%c/styles/general.css%c REL=%cSTYLESHEET%c TYPE=%ctext/css%c>\n",dq,dq,dq,dq,dq,dq);
  fprintf(fp,"</HEAD>\n");
  fprintf(fp,"<BODY>\n");
  fprintf(fp,"<!--#include virtual=%c../html-includes/header.txt%c-->\n",dq,dq);
#else
  fprintf(fp,"<HTML>\n");
  fprintf(fp,"<HEAD>\n");
  htput_title(fp,title);
  fprintf(fp,"<LINK HREF=%c/styles/general.css%c REL=%cSTYLESHEET%c TYPE=%ctext/css%c>\n",dq,dq,dq,dq,dq,dq);
  fprintf(fp,"</HEAD>\n");
  fprintf(fp,"<BODY>\n");
#endif
}


void htput_open_body_1(FILE *fp, const char *title)
{
  fprintf(fp,"<!DOCTYPE HTML PUBLIC %c-//W3O//DTD W3 HTML 2.0//EN%c>\n",dq,dq);
  fprintf(fp,"<HTML>\n");
  fprintf(fp,"<HEAD>\n");
  htput_title(fp,title);
  fprintf(fp,"</HEAD>\n");
  fprintf(fp,"<BODY bgcolor=%c%s%c >\n", dq,HT_BG_COLOR,dq);
  fprintf(fp,"<BASEFONT SIZE=%d >\n\n",HT_BASE_FONT_SIZE);
}


void htput_close_body(FILE *fp)
{

#if defined(HTMLSSI)
  fprintf(fp,"<!--#include virtual=%c../html-includes/footer.txt%c-->\n",dq,dq);
#endif

  fprintf(fp,"</BODY>\n");
  fprintf(fp,"</HTML>\n");
}
 
void htput_separator(FILE *fp)
{
#if defined(HTMLSSI)
  fprintf(fp,"\n");
#else
  htput_open_center(fp);
  fprintf(fp,"<IMG SRC=%c../Icons/%s%c ALIGN=MIDDLE>\n",
	  dq,
	  HT_IMAGE_BAR,
	  dq);
  htput_close_center(fp);
#endif
}

void htput_ndb_address(FILE *fp)
{
#if defined(HTMLSSI)
  fprintf(fp,"\n");
#else
  fprintf(fp,"<BR><BR>\n");
  htput_separator(fp);
  htput_open_center(fp);
  fprintf(fp,"<BR>\n");
  fprintf(fp,"<FONT SIZE=2>\n");
  fprintf(fp,"This HTML page was created using the\n");
  fprintf(fp," <A HREF=%chttp://sw-tools.pdb.org/apps/CIFOBJ%c> CIFOBJ </A> dictionary access tool set\n",dq,dq);
  fprintf(fp,"<BR>\n");
  fprintf(fp," at the\n");              
  fprintf(fp,"<ADDRESS>\n");
  fprintf(fp,"Resource Collaboratory for Structural Bioinformatics <BR>\n");
  fprintf(fp,"Rutgers University, Department of Chemistry<BR>\n");
  fprintf(fp,"New Brunswick, New Jersey<BR>\n");
  fprintf(fp," <A HREF=%cmailto:help@rcsb.rutgers.edu%c> help@rcsb.rutgers.edu </A>\n",dq,dq);
  fprintf(fp,"</ADDRESS>\n");
  fprintf(fp,"</FONT>\n");
  htput_close_center(fp);
#endif
}


void htput_header1_text(FILE *fp,const char *text)
{
  htput_open_center(fp);
  fprintf(fp,"<P><H1>%s</H1>\n",text);
  htput_close_center(fp);
}


void htput_header1_text_with_icon(FILE *fp, const char *text,  const char *icon)
{
  fprintf(fp,"<br><h1><IMG SRC=%c../Icons/%s%c ALIGN=TOP>  %s </h1>\n",
	  dq,
	  icon,
	  dq,text);
}


void htput_header2_text(FILE *fp, const char *text)
{
  htput_open_center(fp);
  fprintf(fp,"<br><H2>%s</H2>\n",text);
  htput_close_center(fp);
}

void htput_header2_text_with_icon(FILE *fp, const char *text,  const char *icon)
{
  fprintf(fp,"<br><h2><IMG SRC=%c../Icons/%s%c ALIGN=TOP> %s </h2>\n",
	  dq,
	  icon,
	  dq,text);
}



void htput_header3_text(FILE *fp,const char *text)
{
  htput_open_center(fp);
  fprintf(fp,"<br><H3>%s</H3>\n",text);
  htput_close_center(fp);
}


void htput_header3_text_with_icon(FILE *fp, const char *text,  const char *icon)
{
  fprintf(fp,"<br><h3><IMG SRC=%c../Icons/%s%c ALIGN=TOP>  %s </h3>\n",
	  dq,
	  icon,
	  dq,text);
}


void htput_paragraph_break(FILE *fp)
{
  fprintf(fp,"<P>\n");
}

void htput_line_break(FILE *fp)
{
  fprintf(fp,"<BR>\n");
}

void htput_text(FILE *fp,const char *text)
{
  fprintf(fp,"%s\n",text);
}

void htput_centered_text(FILE *fp,const char *text)
{
  htput_open_center(fp);  
  fprintf(fp,"%s\n",text);
  htput_close_center(fp);
}


void htput_centered_menu_text(FILE *fp,const char *name)
{
  fprintf(fp,"<BR>\n");
  htput_open_center(fp);
  //  fprintf(fp,"<FONT SIZE=%d> %s </FONT>",HT_MENU_NAME_FONT_SIZE,name);
  fprintf(fp,"%s",name);
  htput_close_center(fp);
}

void htput_centered_label(FILE *fp,const char *label)
{
  fprintf(fp,"<BR>\n");
  htput_open_center(fp);
  //  fprintf(fp,"<FONT SIZE=%d> %s </FONT>",HT_LABEL_FONT_SIZE,label);
  fprintf(fp,"%s",label);
  htput_close_center(fp);
}

#if 0
void htput_centered_menu_file_item(FILE *fp,const char *file, const char *item)
{
  htput_open_center(fp);
  fprintf(fp,"<P><A HREF=%c%s/%s%c><FONT SIZE=%d> %s </FONT></A>",
	 dq,
	 HT_SERVER_NAME,
	 file,
	 dq,HT_MENU_ITEM_FONT_SIZE,item);
  htput_close_center(fp);
}
#endif


void htput_open_table(FILE *fp)
{
  fprintf(fp,"<TABLE>\n");
}

void htput_open_table_with_border(FILE *fp)
{
  fprintf(fp,"<TABLE BORDER>\n");
}


void htput_close_table(FILE *fp)
{
  fprintf(fp,"</TABLE>\n");
}

void htput_open_row(FILE *fp)
{
  fprintf(fp,"<TR>\n");
}


void htput_close_row(FILE *fp)
{
  fprintf(fp,"</TR>\n");
}



void htput_table_col_header(FILE *fp, const char *text)
{
  //  fprintf(fp,"<TH VALIGN=TOP ALIGN=CENTER NOWRAP><FONT SIZE=%d> %s </FONT></TH>\n",
  //              HT_TABLE_HEADER_FONT_SIZE,text);
  fprintf(fp,"<TH VALIGN=TOP ALIGN=CENTER NOWRAP>%s</TH>\n",text);



}

void htput_table_col_text(FILE *fp, const char *text)
{
  //  fprintf(fp,"<TD ALIGN=CENTER><FONT SIZE=%d> %s </FONT></TD>\n",
  //              HT_TABLE_ITEM_FONT_SIZE,text);
  fprintf(fp,"<TD ALIGN=CENTER>%s</TD>\n",text);

}

void htput_table_col_text_left(FILE *fp, const char *text)
{
  //  fprintf(fp,"<TD ALIGN=CENTER><FONT SIZE=%d> %s </FONT></TD>\n",
  //              HT_TABLE_ITEM_FONT_SIZE,text);
  fprintf(fp,"<TD ALIGN=LEFT>%s</TD>\n",text);

}



void htput_table_col_formatted_text(FILE *fp, const char *text)
{
  fprintf(fp,"<TD><pre>\n%s\n</pre></TD>\n",text);

}
 
void htput_table_col_url(FILE *fp, const char *subpath, const char *file, const char *item)
{ 
  //  fprintf(fp,"<TD><A HREF=%c../%s/%s.html%c><FONT SIZE=%d> %s </FONT></A> </TD>\n",
  //	  dq,
  //	  subpath,
  //	  file,
  //	  dq,HT_LIST_ITEM_FONT_SIZE,item);

  fprintf(fp,"<TD ALIGN=LEFT><A HREF=%c../%s/%s.html%c>%s</A> </TD>\n",
	  dq,
	  subpath,
	  file,
	  dq,item);
}

void htput_open_definition_list(FILE *fp)
{
  fprintf(fp,"<DL>\n");
}



void htput_list_item1(FILE *fp, const char *subpath, const char *file, const char *item)
{
  //  fprintf(fp,"<LI><A HREF=%c../%s/%s.html%c><FONT SIZE=%d> %s </FONT></A>\n",
  //	 dq,
  //	 subpath,
  //	 file,
  //	 dq,HT_LIST_ITEM_FONT_SIZE,item);
  fprintf(fp,"<LI><A HREF=%c../%s/%s.html%c>%s</A>\n",
  	 dq,
  	 subpath,
  	 file,
  	 dq,item);
}

void htput_labeled_ref1(FILE *fp, const char *subpath, const char *file, const char *label, const char *item)
{
  fprintf(fp,"<br><h2> %s </h2> <A HREF=%c../%s/%s.html%c> <tt>%s</tt></A> <br>\n",
	  label,
	  dq,
	  subpath,
	  file,
	  dq,
	  item);
}

void htput_labeled_ref1_with_icon(FILE *fp, const char *subpath, const char *file, const char *label, const char *item, const char *icon)
{
  fprintf(fp,"<br><h2><IMG SRC=%c../Icons/%s%c ALIGN=TOP>  %s </h2> <A HREF=%c../%s/%s.html%c>  <tt>%s</tt> </A> \n",
	  dq,
	  icon,dq,
	  label,
	  dq,
	  subpath,
	  file,
	  dq,
	  item);
}


void htput_labeled_text(FILE *fp, const char *label, const char *text)
{
  fprintf(fp,"<br><h2> %s </h2> <tt> %s  </tt><br>\n",label,text);
}

void htput_labeled_text_with_icon(FILE *fp, const char *label, const char *text, const char *icon)
{
  //  fprintf(fp,"<br><h2><IMG SRC=%c../Icons/%s%c ALIGN=TOP> %s </h2> <font size=5><tt> %s </tt></font><br>\n",
  //	  dq,
  //  icon,dq,
  //  label,text);
    fprintf(fp,"<br><h2><IMG SRC=%c../Icons/%s%c ALIGN=TOP> %s </h2> <tt> %s </tt><br>\n",
  	  dq,
    icon,dq,
    label,text);
}

void htput_label_with_icon(FILE *fp, const char *label,  const char *icon)
{
  fprintf(fp,"<br><IMG SRC=%c../Icons/%s%c ALIGN=TOP> <b> %s </b> <br>\n",
	  dq,
	  icon,
	  dq,label);
}


void htput_def_item1(FILE *fp, const char *subpath, const char *file, const char *item, const char *def)
{
  //  fprintf(fp,"<DT><A HREF=%c../%s/%s.html%c><FONT SIZE=%d> %s </A><DD> %s </FONT>\n",
  //	 dq,
  // subpath,
  // file,
  // dq,HT_LIST_ITEM_FONT_SIZE,item,def);
  fprintf(fp,"<DT><A HREF=%c../%s/%s.html%c>%s </A></DT><DD> %s </DD>\n",
	 dq,
	 subpath,
	 file,
	 dq,item,def);
}

void htput_close_definition_list(FILE *fp)
{
  fprintf(fp,"</DL>\n");
}



void htput_open_unordered_list(FILE *fp)
{
  fprintf(fp,"<UL>\n");
}

void htput_close_unordered_list(FILE *fp)
{
  fprintf(fp,"</UL>\n");
}


void htput_pull_down_selection(FILE *fp, const char *label, const char *tag, char **items, int nitems)
{
  int i;
  if (nitems < 0 ) return;
  fprintf(fp,"%s  <SELECT name=%c%s%c> \n",
	 label,
	 dq,tag,dq);

  fprintf(fp,"<OPTION SELECTED> %s\n",items[0]);
  for (i = 1; i < nitems; i++) {
    fprintf(fp,"<OPTION> %s\n", items[i]);
  }
  fprintf(fp,"</SELECT> \n");
}

void htput_scrolled_selection(FILE *fp,const char *label, const char *tag, char **items, 
			      int nitems, int ndisplay)
{
  int i;
  if (nitems < 0 ) return;
  fprintf(fp,"%s  <SELECT NAME=%c%s%c SIZE=%d> \n",
	 label,
	 dq,tag,dq,
	 ndisplay);
  fprintf(fp,"<OPTION SELECTED> %s\n",items[0]);
  for (i = 1; i < nitems; i++) {
    fprintf(fp,"<OPTION> %s\n", items[i]);
  }
  fprintf(fp,"</SELECT> \n");
}


void htput_selection_navigator(FILE *fp, long flags) 
/*
 * Navigation options:
 * 
 * Top Menu             
 * Dictionary Info      
 * Category Groups Menu 
 * Category Menu        
 * Item menu            
 * Data menu
 *
 * This is the navigation line:
 *
 * < Top | Dictionary | Category Groups | Categories | Items | Data>
 */
{
#if defined(HTMLSSI)
  if (flags) fprintf(fp,"\n");
  return;
#else 
  char string[HT_MAX_LINE_LENGTH],url[HT_MAX_URL_LENGTH];
  memset(string,'\0',HT_MAX_LINE_LENGTH);
  sprintf(string,"<br><center>| ");

/*
 * Option: Top Menu 
 */
  if (flags & F_TOP_MENU) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../../index.html%c>Top</A>",
	    dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }


/*
 * Option: Dictionary Info
 */
  if (flags & F_DICT_INFO) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../Index/index.html%c>Dictionary</A>",dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }


/*
 * Option: Category Group Menu 
 */
  if (flags & F_CATEGORY_GROUP_MENU) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../Groups/index.html%c>Category Groups</A>",dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }


/*
 * Option: Category Menu  
 */
  if (flags & F_CATEGORY_MENU) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../Categories/index.html%c>Categories</A>",dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }


/*
 * Option: Item Menu  
 */
  if (flags & F_ITEM_MENU) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../Items/index.html%c>Items</A>",
	    dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }


/*
 * Option: Data Menu  
 */
  if (flags & F_DATA_MENU) {
    memset(url,'\0',HT_MAX_URL_LENGTH);
    sprintf(url,"<A HREF=%c../Data/index.html%c>Data</A>",dq,dq);
    strcat(string,url);
    strcat(string," | ");
  }

  strcat(string," </CENTER>");
  fprintf(fp,"%s\n",string);
#endif
}  



void htput_open_center(FILE *fp)
{
  fprintf(fp,"<CENTER>\n");
}

void htput_close_center(FILE *fp)
{
  fprintf(fp,"</CENTER>\n");
}

void htput_formatted_section(FILE *fp, const char *section, const char *text)
{
  htput_header2_text(fp,section);
  htput_open_preformat(fp);
  fprintf(fp,"\n%s\n",text);
  htput_close_preformat(fp);
}

void htput_formatted_section_with_icon(FILE *fp, const char *section, const char *text, const char *icon)
{
  htput_header2_text_with_icon(fp,section,icon);
  htput_open_preformat(fp);
  fprintf(fp,"\n%s\n",text);
  htput_close_preformat(fp);
}


void htput_example_section(FILE *fp, const string& description, const char *text, int numb)
{
  char cbuf[30];
  memset(cbuf,'\0',30);
  sprintf(cbuf,"Example %d:",numb);
  htput_header3_text(fp,cbuf);

  if (!CifString::IsEmptyValue(description))
  {
/*    fprintf(fp,"<center><i>%s</i> </center>\n",description); */
    htput_open_preformat(fp);
    fprintf(fp,"%s",description.c_str());
    htput_close_preformat(fp);
    htput_line_break(fp);
  }
  htput_open_preformat(fp);
  fprintf(fp,"%s",text);
  htput_close_preformat(fp);
}


void htput_example_section_with_icon(FILE *fp, const string& description, const char *text, int numb, const char *icon)
{
  char cbuf[30];
  memset(cbuf,'\0',30);
  sprintf(cbuf,"Example %d:",numb);
  htput_header3_text_with_icon(fp,cbuf,icon);

  if (!CifString::IsEmptyValue(description)) {
/*     fprintf(fp,"<center><i>%s</i> </center>\n",description); */
    htput_open_preformat(fp);
    fprintf(fp,"%s",description.c_str());
    htput_close_preformat(fp);
    htput_line_break(fp);
  }
  htput_open_preformat(fp);
  fprintf(fp,"%s",text);
  htput_close_preformat(fp);
}


void htput_open_preformat(FILE *fp)
{
  //  fprintf(fp,"<PRE>\n");
  fprintf(fp,"<PRE>");
}

void htput_close_preformat(FILE *fp)
{
  fprintf(fp,"</PRE>\n");
}

void ht_strip_nl(char *string)
{
   int i;
   i = strlen(string);
   if (string[i-1] == '\n') string[i-1] = '\0';
}

