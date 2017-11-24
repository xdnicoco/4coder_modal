#if !defined(NJ_MODE_CHORD_SETTINGS)
#define NJ_MODE_CHORD_SETTINGS

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

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
    bind(context, 's', MDFR_NONE, nj_chord_settings_toggle_scrollbar_then_prev); 
    bind(context, 'f', MDFR_NONE, nj_chord_settings_toggle_fullscreen_then_prev); 
    bind(context, 'l', MDFR_NONE, nj_chord_settings_toggle_line_wrap_then_prev); 
    bind(context, 'v', MDFR_NONE, nj_chord_settings_toggle_virtual_whitespace_then_prev); 
    bind(context, '?', MDFR_NONE, nj_chord_settings_toggle_show_whitespace_then_prev); 
    bind(context, 'q', MDFR_NONE, exit_4coder); 
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

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_scrollbar_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's scrollbar, then go back to the previous mode."){
    exec_command(app, toggle_scrollbar);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_filebar_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's filebar, then go back to the previous mode."){
    exec_command(app, toggle_filebar);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_fullscreen_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's fullscreen, then go back to the previous mode."){
    exec_command(app, toggle_fullscreen);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_line_wrap_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's line wrap, then go back to the previous mode."){
    exec_command(app, toggle_line_wrap);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_virtual_whitespace_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's virtual whitespace, then go back to the previous mode."){
    exec_command(app, toggle_virtual_whitespace);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_settings_toggle_show_whitespace_then_prev)
CUSTOM_DOC("Toggles the visibility status of the current view's whitespace, then go back to the previous mode."){
    exec_command(app, toggle_show_whitespace);
    exec_command(app, nj_activate_previous_mode);
}

#endif // NJ_MODE_CHORD_SETTINGS