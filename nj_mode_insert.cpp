#if !defined(NJ_MODE_INSERT_CPP)
#define NJ_MODE_INSERT_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
#if 1
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x100f05, // color_bg
                             0x711400, // color_bar
                             0x752410, // color_bar_hover
                             0x793420, // color_bar_active
                             0x793420, // color_mode
                             0x8d360d, // color_mark
                             0x55cccc, // color_pop1
                             0x50080f  // color_pop2
                             );
#else
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x150f05, // color_bg
                             0x813410, // color_bar
                             0x854420, // color_bar_hover
                             0x895430, // color_bar_active
                             0x895430, // color_mode
                             0x8d360d, // color_mark
                             0x030c7f, // color_pop1
                             0x50080f  // color_pop2
                             );
#endif
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_newline_then_insert_before)
CUSTOM_DOC("Inserts a newline before the line under the cursor, then activates insert mode."){
    seek_beginning_of_line(app);
    write_string(app, make_lit_string("\n"));
    move_left(app);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_newline_then_insert_after)
CUSTOM_DOC("Inserts a newline after the line under the cursor, then activates insert mode."){
    seek_end_of_line(app);
    write_string(app, make_lit_string("\n"));
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_insert_after)
CUSTOM_DOC("Moves to the next character in the current line, then activates insert mode."){
    View_Summary view = get_active_view(app, AccessOpen);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, AccessOpen);
    char nextch[2];
    int pos = view.cursor.pos;
    buffer_read_range(app, &buffer, pos, pos + 1, nextch);
    if (nextch[0] != '\n') {
        move_right(app);
    }
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_seek_eol_then_insert)
CUSTOM_DOC("Moves to the end of the current line, then activates insert mode."){
    seek_end_of_textual_line(app);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

#define nj_bind_mode_keys_insert(context) \
begin_map(context, mapid_insert); \
bind(context, '\t', MDFR_NONE,  word_complete); \
bind(context, '\t', MDFR_CTRL,  auto_tab_range); \
bind(context, '\t', MDFR_SHIFT, auto_tab_line_at_cursor); \
bind(context, key_back, MDFR_SHIFT, backspace_char); \
bind_vanilla_keys(context, write_character); \
inherit_map(context, mapid_common); \
end_map(context); // mapid_insert


#endif // NJ_MODE_INSERT_CPP