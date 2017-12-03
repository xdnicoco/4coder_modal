/*
This mode sets some of the editor, view, buffer or mode settings, then activates the previous mode.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_CHORD_SETTINGS)
#define _MODE_CHORD_SETTINGS

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x000000, // color_bg
                             0x000000, // color_bar
                             0x041010, // color_bar_hover
                             0x082020, // color_bar_active
                             0x666666, // color_mode
                             0x343434, // color_mark
                             0x55cccc, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_settings)
CUSTOM_DOC("Activates 'chord_settings' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind(context, 'b', MDFR_NONE, nj_chord_settings_toggle_filebar_then_prev);
    bind(context, 'c', MDFR_NONE, nj_chord_settings_clean_lines_then_prev);
    bind(context, 's', MDFR_NONE, nj_chord_settings_toggle_scrollbar_then_prev);
    bind(context, 'f', MDFR_NONE, nj_chord_settings_toggle_fullscreen_then_prev);
    bind(context, 'l', MDFR_NONE, nj_chord_settings_toggle_line_wrap_then_prev);
    bind(context, 'i', MDFR_NONE, nj_chord_settings_invert_colors_then_prev);
    bind(context, 'I', MDFR_NONE, nj_chord_settings_toggle_buffer_importance_then_prev);
    bind(context, 't', MDFR_NONE, nj_chord_settings_open_color_tweaker_then_prev);
    bind(context, 'v', MDFR_NONE, nj_chord_settings_toggle_virtual_whitespace_then_prev);
    bind(context, '?', MDFR_NONE, nj_chord_settings_toggle_show_whitespace_then_prev);
    bind(context, 'q', MDFR_NONE, exit_4coder);
    
    bind(context, key_down,  MDFR_NONE, nj_move_panel_down_then_prev);
    bind(context, key_right, MDFR_NONE, nj_move_panel_right_then_prev);
    
    bind(context, '\n',    MDFR_NONE, nj_mode_enter_normal);
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    end_map(context);
}

CUSTOM_COMMAND_SIG(toggle_scrollbar)
CUSTOM_DOC("Toggles the visibility status of the current view's scrollbar."){
    View_Summary view = get_active_view(app, AccessAll);
    bool32 value;
    view_get_setting(app, &view, ViewSetting_ShowScrollbar, &value);
    view_set_setting(app, &view, ViewSetting_ShowScrollbar, !value);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_open_color_tweaker_then_prev)
CUSTOM_DOC("Opens the 4coder color tweaker, then go back to the previous mode."){
    open_color_tweaker(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_scrollbar_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's scrollbar, then go back to the previous mode."){
    toggle_scrollbar(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_filebar_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's filebar, then go back to the previous mode."){
    toggle_filebar(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_fullscreen_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's fullscreen, then go back to the previous mode."){
    toggle_fullscreen(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_line_wrap_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's line wrap, then go back to the previous mode."){
    toggle_line_wrap(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_virtual_whitespace_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's virtual whitespace, then go back to the previous mode."){
    toggle_virtual_whitespace(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_show_whitespace_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's whitespace, then go back to the previous mode."){
    toggle_show_whitespace(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_clean_lines_then_prev)
CUSTOM_DOC("Removes trailing whitespace from all lines in the current buffer, then go back to previous mode."){
    clean_all_lines(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_set_modal_color_theme){
    // NOTE(NJ): The following color theme is tuned to work best with the mode color scheme
    // modifications when switching modes.
    int_color color_bg                = 0xFF1F1F1F;
    int_color color_margin            = 0xFF262626;
    int_color color_margin_hover      = 0xFF333333;
    int_color color_margin_active     = 0xFF404040;
    int_color color_cursor            = 0xFFE95420;
    int_color color_highlight         = 0xFF703419;
    int_color color_mark              = 0xFF8D360d;
    int_color color_default           = 0xFFDADACA;
    int_color color_at_highlight      = 0xFFCDAA7D;
    int_color color_comment           = 0xFF7E9E7E;
    int_color color_keyword           = 0xFFF5C59b;
    int_color color_str_constant      = 0xFFCD9494;
    int_color color_int_constant      = 0xFFCAB080;
    int_color color_preproc           = 0xFF6BE0E3;
    int_color color_special_character = 0xFFFF0000;
    int_color color_ghost_character   = 0xFF5B4D3C;
    int_color color_paste             = 0xFF2DFFAB;
    int_color color_undo              = 0xFFE6644D;
    int_color color_highlight_junk    = 0xFF3A0000;
    int_color color_highlight_white   = 0xFF003A3A;
    int_color color_bar               = 0xFF30333F;
    int_color color_bar_active        = 0xFF000000;
    int_color color_base              = 0xFFAAAAAA;
    int_color color_pop1              = 0xFF03CF0C;
    int_color color_pop2              = 0xFFFF0000;
    
    Theme_Color colors[] =  {
        { Stag_Back,              color_bg},
        { Stag_Margin,            color_margin},
        { Stag_Margin_Hover,      color_margin_hover},
        { Stag_Margin_Active,     color_margin_active},
        
        { Stag_List_Item,         color_margin},
        { Stag_List_Item_Hover,   color_margin_hover},
        { Stag_List_Item_Active,  color_margin_active},
        { Stag_Cursor,            color_cursor },
        { Stag_Highlight,         color_highlight },
        { Stag_Mark,              color_mark },
        { Stag_Default,           color_default },
        { Stag_At_Cursor,         color_default },
        { Stag_At_Highlight,      color_at_highlight },
        { Stag_Comment,           color_comment },
        { Stag_Keyword,           color_keyword },
        { Stag_Str_Constant,      color_str_constant },
        { Stag_Char_Constant,     color_str_constant },
        { Stag_Int_Constant,      color_int_constant },
        { Stag_Float_Constant,    color_int_constant },
        { Stag_Bool_Constant,     color_int_constant },
        { Stag_Include,           color_str_constant },
        { Stag_Preproc,           color_preproc },
        { Stag_Special_Character, color_special_character },
        { Stag_Ghost_Character,   color_ghost_character },
        
        { Stag_Paste,             color_paste },
        
        { Stag_Undo,              color_undo },
        
        { Stag_Highlight_Junk,    color_highlight_junk },
        { Stag_Highlight_White,   color_highlight_white },
        
        { Stag_Bar,               color_bar },
        { Stag_Bar_Active,        color_bar_active },
        { Stag_Base,              color_base },
        { Stag_Pop1,              color_pop1 },
        { Stag_Pop2,              color_pop2 },
    };
    
    if(nj_theme_colors_inverted) {
        nj_invert_colors(colors, ArrayCount(colors));
    }
    set_theme_colors(app, colors, ArrayCount(colors));
    // change_theme(app, literal("NJ Modal Theme"));
}

CUSTOM_COMMAND_SIG(nj_chord_settings_invert_colors_then_prev)
CUSTOM_DOC("Inverts the theme colors, then go back to previous mode."){
    nj_theme_colors_inverted = !nj_theme_colors_inverted;
    nj_set_modal_color_theme(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_move_panel_down_then_prev)
CUSTOM_DOC("Closes the current panel and re opens it bellow the current panel"){
    uint32_t access = AccessAll;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    close_panel(app);
    open_panel_hsplit(app);
    
    View_Summary new_view = get_active_view(app, access);
    view_set_buffer(app, &new_view, buffer.buffer_id, buffer.lock_flags);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_move_panel_right_then_prev)
CUSTOM_DOC("Closes the current panel and re opens it bellow the current panel"){
    uint32_t access = AccessAll;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    close_panel(app);
    open_panel_vsplit(app);
    
    View_Summary new_view = get_active_view(app, access);
    view_set_buffer(app, &new_view, buffer.buffer_id, buffer.lock_flags);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_buffer_importance_then_prev)
CUSTOM_DOC("Inverts the theme colors, then go back to previous mode."){
    uint32_t access = AccessAll;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    int32_t buffer_is_unimportant;
    buffer_get_setting(app, &buffer, BufferSetting_Unimportant, &buffer_is_unimportant);
    buffer_set_setting(app, &buffer, BufferSetting_Unimportant, !buffer_is_unimportant);
    nj_activate_previous_mode(app);
}

#endif // _MODE_CHORD_SETTINGS