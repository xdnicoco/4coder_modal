#if !defined(NJ_MODE_CHORD_REPLACE_SINGLE_CPP)
#define NJ_MODE_CHORD_REPLACE_SINGLE_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x050f15, // color_bg
                             0x032459, // color_bar
                             0x1c338e, // color_bar_active
                             0x1e439e, // color_bar_hover
                             0xe91e63, // color_mode
                             0xc30c4a, // color_mark
                             0x03cf0c, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_replace_character_then_prev)
CUSTOM_DOC("Replaces the character under the cursor with the pressed character, then return to the previous mode."){
    exec_command(app, nj_replace_character);
    exec_command(app, move_left);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_mode_chord_replace_single_paste_then_prev)
CUSTOM_DOC("Removes the character under the cursor, then paste from the clipboard and return to the previous mode."){
    uint32_t access = AccessProtected;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    if(buffer_get_char(app, &buffer, view.cursor.pos) != '\n') {
        delete_char(app);
    }
    exec_command(app, paste);
    exec_command(app, nj_activate_previous_mode);
}


#define nj_bind_mode_keys_chord_replace_single(context)\
begin_map(context, mapid_chord_replace_single); \
inherit_map(context, mapid_nomap); \
bind_vanilla_keys(context, nj_replace_character_then_prev); \
bind(context, 'v', MDFR_CTRL, nj_mode_chord_replace_single_paste_then_prev); \
bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal); \
end_map(context);


#endif // NJ_MODE_CHORD_REPLACE_SINGLE_CPP
