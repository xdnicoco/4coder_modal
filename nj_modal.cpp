/*
 nj 4coder custom configuration
 This is the main file that binds all the modal functions.
This is an experimental way for me to use file in terms of modules and it's far from perfect,
but that's how I do it.
Also, some of the bindings are specified here (that is bindings that are not for the keymap id
of one of the modes).

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/


#if !defined(FCODER_NJ_CPP)
#define FCODER_NJ_CPP
//
// Key Bindings
//

static bool32 nj_theme_colors_inverted = false;

static void nj_invert_colors(Theme_Color *colors, int32_t len){
    for(int i = 0; i < len; ++i) {
        colors[i].color = (0xFFFFFFFF - colors[i].color) | 0xFF000000;
    }
}

#include "4coder_generated/command_metadata.h"
#define NJ_MODES(modifier) \
modifier(insert)\
modifier(replace)\
modifier(chord_replace_single)\
modifier(chord_insert_single)\
modifier(chord_snippets)\
modifier(chord_settings)\
modifier(chord_case)\
modifier(chord_goto)

enum NJ_Mapid {
    mapid_normal = mapid_global,
    mapid_movements = 80000,
    mapid_common,
    
#define NJ_MODE_MAPID(mode) mapid_##mode,
    NJ_MODES(NJ_MODE_MAPID)
#undef NJ_MODE_MAPID
};


#define NJ_MODE_MAPID_(mode) mapid_##mode
#define NJ_MODE_MAPID(mode) NJ_MODE_MAPID_(mode)

NJ_Mapid nj_previous_mapid;

#define NJ_MODE_STATE_(mode) nj_mode_state_##mode
#define NJ_MODE_STATE(mode)  NJ_MODE_STATE_(mode)

#define NJ_MODE_STATE_DECLERATION_(mode) NJ_Mode_State_##mode
#define NJ_MODE_STATE_DECLERATION(mode)  NJ_MODE_STATE_DECLERATION_(mode)

#define NJ_MODE_BIND_DECLERATION_(mode) inline void nj_bind_mode_keys_##mode(Bind_Helper *context)
#define NJ_MODE_BIND_DECLERATION(mode)  NJ_MODE_BIND_DECLERATION_(mode)

#define NJ_MODE_PRINT_ENTER_FUNCTION_(mode, color_bg, color_bar, color_bar_hover, color_bar_active, color_mode, color_mark, color_pop1, color_pop2)     \
NJ_MODE_STATE_DECLERATION(mode) NJ_MODE_STATE(mode) = {};                                                                                               \
static void mode_enter_##mode(struct Application_Links *app, int buffer_id){                                                                            \
    unsigned int access = AccessAll;                                                                                                                    \
    Buffer_Summary buffer;                                                                                                                              \
    buffer = get_buffer(app, buffer_id, access);                                                                                                        \
    NJ_Mapid current_mapid;                                                                                                                             \
    buffer_get_setting(app, &buffer, BufferSetting_MapID, (int32_t *)&current_mapid);                                                                   \
    if(current_mapid != mapid_##mode) nj_previous_mapid = current_mapid;                                                                                \
    buffer_set_setting(app, &buffer, BufferSetting_MapID, mapid_##mode);                                                                                \
    Theme_Color colors[] =  {                                                                                                                           \
        { Stag_Back,          color_bg         },                                                                                                       \
        { Stag_Margin,        color_bar        },                                                                                                       \
        { Stag_Margin_Hover,  color_bar_hover  },                                                                                                       \
        { Stag_Margin_Active, color_bar_active },                                                                                                       \
        { Stag_Bar,           color_bar        },                                                                                                       \
        { Stag_Bar_Active,    color_bar_active },                                                                                                       \
        { Stag_Cursor,        color_mode       },                                                                                                       \
        { Stag_Mark,          color_mark       },                                                                                                       \
        { Stag_Pop1,          color_pop1       },                                                                                                       \
        { Stag_Pop2,          color_pop2       },                                                                                                       \
    };                                                                                                                                                  \
    if(nj_theme_colors_inverted) nj_invert_colors(colors, ArrayCount(colors));                                                                          \
    set_theme_colors(app, colors, ArrayCount(colors));                                                                                                  \
    NJ_MODE_PRINT_ENTER_HOOK;                                                                                                                           \
}
#define NJ_MODE_PRINT_ENTER_FUNCTION(mode, color_bg, color_bar, color_bar_hover, color_bar_active, color_mode, color_mark, color_pop1, color_pop2)      \
NJ_MODE_PRINT_ENTER_FUNCTION_(mode, color_bg, color_bar, color_bar_hover, color_bar_active, color_mode, color_mark, color_pop1, color_pop2)

#define NJ_MODE_ACTIVATE_ENTER_FUNCTION_(mode) mode_enter_##mode(app, get_active_view(app, AccessAll).buffer_id);
#define NJ_MODE_ACTIVATE_ENTER_FUNCTION(mode) NJ_MODE_ACTIVATE_ENTER_FUNCTION_(mode)

#define STRINGIZE_TOKEN(token) #token
#define NJ_INCLUDE_MODE_FILE_(mode) STRINGIZE_TOKEN(nj_mode_##mode.cpp)
#define NJ_INCLUDE_MODE_FILE(mode)  NJ_INCLUDE_MODE_FILE_(mode)

inline void nj_activate_mode_by_mapid(Application_Links *app, NJ_Mapid mapid);
inline char *nj_get_mode_name_by_mapid(Application_Links *app, NJ_Mapid mapid);

CUSTOM_COMMAND_SIG(nj_activate_previous_mode)
CUSTOM_DOC("Activate the mode previous to the current mode.")
{
    nj_activate_mode_by_mapid(app, nj_previous_mapid);
}

#define NJ_CURRENT_MODE normal // normal mode is essential
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE insert
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE replace
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_replace_single
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_insert_single
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_snippets
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_case
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_settings
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#define NJ_CURRENT_MODE chord_goto
#include NJ_INCLUDE_MODE_FILE(NJ_CURRENT_MODE)
#undef NJ_CURRENT_MODE

#undef NJ_MODE_PRINT_ENTER_FUNCTION_
#undef NJ_MODE_PRINT_ENTER_FUNCTION
#undef NJ_INCLUDE_MODE_FILE

#define NJ_GEN_NAMED_BINDINGS(mode) {make_lit_string(#mode), nj_mode_enter_##mode},
static Named_Mapping nj_named_maps_values[] = {
    NJ_GEN_NAMED_BINDINGS(normal)
        NJ_MODES(NJ_GEN_NAMED_BINDINGS)
};
#undef NJ_GEN_NAMED_BINDINGS

inline void nj_activate_mode_by_mapid(Application_Links *app, NJ_Mapid mapid){
    switch(mapid)
    {
#define NJ_GEN_MAPID_MODE_PAIR(mode) case mapid_##mode: {NJ_MODE_ACTIVATE_ENTER_FUNCTION(mode);}break;
        default:
        NJ_GEN_MAPID_MODE_PAIR(normal)
            NJ_MODES(NJ_GEN_MAPID_MODE_PAIR)
#undef NJ_GEN_MAPID_MODE_PAIR
    }
}

inline char *nj_get_mode_name_by_mapid(Application_Links *app, NJ_Mapid mapid){
    switch(mapid)
    {
#define NJ_GEN_MAPID_MODE_NAME_PAIR(mode) case mapid_##mode: {return (#mode);}break;
        NJ_GEN_MAPID_MODE_NAME_PAIR(normal)
            NJ_MODES(NJ_GEN_MAPID_MODE_NAME_PAIR)
#undef NJ_GEN_MAPID_MODE_NAME_PAIR
    }
}


#define MDFR_CTLT MDFR_CTRL  | MDFR_ALT
#define MDFR_SHRL MDFR_SHIFT | MDFR_CTRL
#define MDFR_SHLT MDFR_SHIFT | MDFR_ALT

void
nj_keys(Bind_Helper *context){
    begin_map(context, mapid_common);
    bind(context, key_f4, MDFR_ALT,  exit_4coder);
    bind(context, 'q',    MDFR_CTRL, exit_4coder);
    
    bind(context, ' ', MDFR_ALT, nj_toggler);
    
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    
    bind(context, '=', MDFR_CTRL, open_panel_vsplit);
    bind(context, '-', MDFR_CTRL, open_panel_hsplit);
    bind(context, '+', MDFR_CTRL, close_panel);
    bind(context, '_', MDFR_CTRL, close_panel);
    
    bind(context, '=', MDFR_CTLT, increase_face_size);
    bind(context, '-', MDFR_CTLT, decrease_face_size);
    
    // NOTE(allen|a4.0.7): You can now bind left and rig.ht clicks.
    // They only trigger on mouse presses.  Modifiers do work
    // so control+click shift+click etc can now have special meanings.
    bind(context, key_mouse_left,  MDFR_NONE, click_set_cursor);
    bind(context, key_mouse_right, MDFR_NONE, click_set_mark);
    
    // NOTE(allen|a4.0.11): You can now bind left and right mouse
    // button releases.  Modifiers do work so control+release shift+release
    // etc can now have special meanings.
    bind(context, key_mouse_left_release, MDFR_NONE, click_set_mark);
    
    bind(context, key_left,      MDFR_NONE,  move_left);
    bind(context, key_right,     MDFR_NONE,  move_right);
    bind(context, key_del,       MDFR_NONE,  delete_char);
    bind(context, key_del,       MDFR_SHIFT, delete_char);
    bind(context, key_back,      MDFR_NONE,  backspace_char);
    bind(context, key_back,      MDFR_SHRL,  nj_backspace_line);
    bind(context, key_up,        MDFR_NONE,  move_up);
    bind(context, key_down,      MDFR_NONE,  move_down);
    bind(context, key_end,       MDFR_NONE,  seek_end_of_textual_line);
    bind(context, key_end,       MDFR_CTRL,  seek_end_of_line);
    bind(context, key_end,       MDFR_ALT,   nj_seek_end_of_file);
    bind(context, key_home,      MDFR_NONE,  seek_beginning_of_textual_line);
    bind(context, key_home,      MDFR_CTRL,  seek_beginning_of_line);
    bind(context, key_home,      MDFR_ALT,   nj_seek_beginning_of_file);
    bind(context, key_page_up,   MDFR_NONE,  page_up);
    bind(context, key_page_down, MDFR_NONE,  page_down);
    
    bind(context, key_left,  MDFR_CTRL, seek_alphanumeric_or_camel_left);
    bind(context, key_left,  MDFR_ALT,  seek_whitespace_left);
    
    bind(context, key_up,   MDFR_CTRL, seek_whitespace_up_end_line);
    bind(context, key_up,   MDFR_ALT,  move_up_10);
    bind(context, key_up,   MDFR_SHRL, move_line_up);
    
    bind(context, key_down, MDFR_CTRL, seek_whitespace_down_end_line);
    bind(context, key_down, MDFR_ALT,  move_down_10);
    bind(context, key_down, MDFR_SHRL, move_line_down);
    
    bind(context, key_right, MDFR_CTRL, seek_alphanumeric_or_camel_right);
    bind(context, key_right, MDFR_ALT,  seek_whitespace_right);
    
    bind(context, key_back, MDFR_CTRL, nj_backspace_word);
    bind(context, key_del,  MDFR_CTRL, delete_word);
    bind(context, key_back, MDFR_ALT, snipe_token_or_word);
    
    bind(context, '\t', MDFR_NONE,  word_complete);
    
    bind(context, ' ',  MDFR_CTRL, set_mark);
    bind(context, ';',  MDFR_CTRL, cursor_mark_swap);
    bind(context, ';',  MDFR_ALT,  nj_chord_snippet_eol_semicolon);
    bind(context, '\\', MDFR_ALT,  nj_chord_snippet_eol_backslash);
    bind(context, 'c',  MDFR_CTRL, copy);
    bind(context, '2',  MDFR_ALT,  nj_open_matching_file_cpp_current_panel);
    bind(context, '@',  MDFR_ALT,  open_matching_file_cpp);
    bind(context, 'a',  MDFR_CTRL, select_all);
    bind(context, 'a',  MDFR_ALT,  nj_ocd_equals);
    bind(context, 'a',  MDFR_CTLT, nj_ocd_arbitrary);
    bind(context, 'd',  MDFR_ALT,  delete_range);
    bind(context, 'D',  MDFR_ALT,  delete_line);
    bind(context, 'd',  MDFR_CTLT, nj_delete_rectangle);
    bind(context, 'd',  MDFR_CTRL, duplicate_line);
    bind(context, 'e',  MDFR_CTRL, center_view);
    bind(context, 'E',  MDFR_CTRL, left_adjust_view);
    bind(context, 'e',  MDFR_ALT,  nj_snipe_token_or_word);
    bind(context, 'f',  MDFR_CTRL, toggle_fullscreen);
    bind(context, 'g',  MDFR_ALT,  nj_mode_enter_chord_goto);
    
    bind(context, 'm', MDFR_CTRL, cursor_mark_swap);
    bind(context, 'p', MDFR_ALT,  load_project);
    bind(context, 'P', MDFR_ALT,  close_all_code);
    bind(context, 'R', MDFR_ALT,  reopen);
    bind(context, 'r', MDFR_ALT,  replace_in_range);
    bind(context, 'r', MDFR_CTLT, nj_replace_rectangle);
    
    // bind(context, 'c', MDFR_CTLT, nj_copy_rectangle);
    // bind(context, 'x', MDFR_CTLT, nj_cut_rectangle);
    // bind(context, 'v', MDFR_CTLT, nj_paste_rectangle);
    
    bind(context, 'R', MDFR_CTRL, query_replace_identifier);
    bind(context, 'r', MDFR_CTRL, query_replace);
    bind(context, 's', MDFR_ALT,  save);
    bind(context, 's', MDFR_CTRL, search);
    bind(context, 'S', MDFR_CTRL, search_identifier);
    bind(context, 't', MDFR_ALT,  list_all_substring_locations_case_insensitive);
    bind(context, 'T', MDFR_ALT,  list_all_locations_of_identifier);
    bind(context, 'T', MDFR_CTRL, list_all_locations);
    bind(context, 'v', MDFR_CTRL, paste_and_indent);
    bind(context, 'V', MDFR_CTRL, paste_next_and_indent);
    bind(context, 'p', MDFR_CTRL, paste_and_indent);
    bind(context, 'P', MDFR_CTRL, paste_next_and_indent);
    bind(context, 'x', MDFR_CTRL, cut);
    bind(context, 'Z', MDFR_CTRL, redo);
    bind(context, 'z', MDFR_CTRL, undo);
    
    bind(context, '1', MDFR_ALT, open_file_in_quotes);
    bind(context, '2', MDFR_ALT, open_matching_file_cpp);
    
    bind(context, '2', MDFR_CTRL, decrease_line_wrap);
    bind(context, '3', MDFR_CTRL, increase_line_wrap);
    
    bind(context, '3', MDFR_ALT,  nj_mode_enter_chord_snippets);
    
    bind(context, '4', MDFR_ALT,  nj_mode_enter_chord_case);
    
    bind(context, '8', MDFR_ALT,  nj_mode_enter_chord_settings);
    
    bind(context, '!', MDFR_ALT,  nj_execute_any_cli);
    bind(context, '~', MDFR_CTRL, nj_execute_arbitrary_command);
    bind(context, ':', MDFR_CTRL, nj_execute_arbitrary_command);
    
    bind(context, '\n', MDFR_NONE,  newline_or_goto_position);
    bind(context, '\n', MDFR_SHIFT, newline_or_goto_position_same_panel);
    bind(context, ' ',  MDFR_SHIFT, write_character);
    bind(context, '_',  MDFR_NONE,  write_character);
    bind(context, '-',  MDFR_ALT,   nj_write_arrow);
    bind(context, '.',  MDFR_ALT,   nj_toggler);
    
    bind(context, '[', MDFR_CTRL, open_long_braces);
    bind(context, '{', MDFR_CTRL, open_long_braces_semicolon);
    bind(context, '}', MDFR_CTRL, open_long_braces_break);
    
    bind(context, '/', MDFR_CTRL, nj_toggle_comment_line);
    bind(context, '/', MDFR_ALT,  if0_off);
    
    bind(context, key_f1,  MDFR_NONE, project_fkey_command);
    bind(context, key_f2,  MDFR_NONE, project_fkey_command);
    bind(context, key_f3,  MDFR_NONE, project_fkey_command);
    bind(context, key_f4,  MDFR_NONE, project_fkey_command);
    
    bind(context, key_f5,  MDFR_NONE, project_fkey_command);
    bind(context, key_f6,  MDFR_NONE, project_fkey_command);
    bind(context, key_f7,  MDFR_NONE, project_fkey_command);
    bind(context, key_f8,  MDFR_NONE, project_fkey_command);
    
    bind(context, key_f9,  MDFR_NONE, project_fkey_command);
    bind(context, key_f10, MDFR_NONE, project_fkey_command);
    bind(context, key_f11, MDFR_NONE, project_fkey_command);
    bind(context, key_f12, MDFR_NONE, project_fkey_command);
    
    bind(context, key_f13, MDFR_NONE, project_fkey_command);
    bind(context, key_f14, MDFR_NONE, project_fkey_command);
    bind(context, key_f15, MDFR_NONE, project_fkey_command);
    bind(context, key_f16, MDFR_NONE, project_fkey_command);
    
    end_map(context); // mapid_common
    
    begin_map(context, mapid_movements);
    inherit_map(context, mapid_common);
    bind_vanilla_keys(context, cmdid_null);
    
    bind(context, 'h', MDFR_NONE, move_left);
    bind(context, 'h', MDFR_CTRL, seek_alphanumeric_or_camel_left);
    bind(context, 'h', MDFR_ALT,  seek_whitespace_left);
    
    bind(context, 'j', MDFR_NONE, move_down);
    bind(context, 'j', MDFR_ALT,  move_down_10);
    bind(context, 'j', MDFR_CTRL, seek_whitespace_down_end_line);
    bind(context, 'j', MDFR_CTLT, move_line_down);
    
    bind(context, 'k', MDFR_NONE, move_up);
    bind(context, 'k', MDFR_ALT,  move_up_10);
    bind(context, 'k', MDFR_CTRL, seek_whitespace_up_end_line);
    bind(context, 'k', MDFR_CTLT, move_line_up);
    
    bind(context, 'l', MDFR_NONE, move_right);
    bind(context, 'l', MDFR_CTRL, seek_alphanumeric_or_camel_right);
    bind(context, 'l', MDFR_ALT,  seek_whitespace_right);
    
    bind(context, 'w', MDFR_NONE, change_active_panel);
    bind(context, 'W', MDFR_NONE, change_active_panel_backwards);
    
    bind(context, '/', MDFR_NONE, search);
    bind(context, 'n', MDFR_NONE, goto_next_error);
    bind(context, 'N', MDFR_NONE, goto_prev_error);
    bind(context, 'm', MDFR_NONE, goto_first_error);
    
    bind(context, '9', MDFR_NONE, seek_end_of_textual_line);
    bind(context, '9', MDFR_CTRL, seek_end_of_line);
    bind(context, '9', MDFR_ALT,  nj_seek_end_of_file);
    bind(context, '$', MDFR_NONE, seek_end_of_textual_line);
    bind(context, '$', MDFR_CTRL, seek_end_of_line);
    bind(context, '$', MDFR_ALT,  nj_seek_end_of_file);
    
    bind(context, '0', MDFR_NONE, seek_beginning_of_textual_line);
    bind(context, '0', MDFR_CTRL, seek_beginning_of_line);
    bind(context, '0', MDFR_ALT,  nj_seek_beginning_of_file);
    
    bind(context, '=', MDFR_NONE, change_to_build_panel);
    bind(context, '-', MDFR_NONE, close_build_panel);
    
    bind(context, '.', MDFR_NONE, nj_toggler);
    
    bind(context, key_insert, MDFR_NONE, nj_mode_enter_replace);
    
    bind(context, ';', MDFR_NONE, cursor_mark_swap);
    
    end_map(context); // mapid_movements
    
#define NJ_GEN_MODE_KEYMAP(mode) nj_bind_mode_keys_##mode(context);
    NJ_GEN_MODE_KEYMAP(normal)
        NJ_MODES(NJ_GEN_MODE_KEYMAP)
#undef NJ_GEN_MODE_KEYMAP
}

#endif // FCODER_NJ_CPP
