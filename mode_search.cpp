#if !defined(_MODE_SEARCH_CPP)
#define _MODE_SEARCH_CPP

#define NJ_MODE_PRINT_ENTER_HOOK \
View_Summary view = get_active_view(app, AccessAll);\
nj_mode_search_state.last_position = view.cursor;\
nj_mode_search_state.position = nj_mode_search_state.last_position.pos;\
nj_mode_search_state.match = make_range(nj_mode_search_state.position, nj_mode_search_state.position); \
nj_mode_search_state.bar.string = make_fixed_width_string(nj_mode_search_state_string_space); \
nj_mode_search_state.bar.prompt = make_lit_string("Search: "); \
nj_mode_search_state.reverse = false; \
nj_mode_search_state.step = false; \
if (start_query_bar(app, &nj_mode_search_state.bar, 0) == 0) return;

NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x051510, // color_bg
                             0x034449, // color_bar
                             0x021f4f, // color_bar_hover
                             0x055459, // color_bar_active
                             0x207469, // color_mode
                             0x0d6695, // color_mark
                             0x03cf0c, // color_pop1
                             0xff0000  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_search)
CUSTOM_DOC("Activates 'search' mode.")
{
    NJ_ENTER_MODE(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    
    bind_vanilla_keys(context, nj_mode_search_append_char);
    bind(context, key_back, MDFR_NONE, nj_mode_search_backspace_char);
    
    bind(context, key_up, MDFR_NONE, nj_mode_search_step_backward);
    bind(context, key_down, MDFR_NONE, nj_mode_search_step_forward);
    bind(context, key_page_up, MDFR_NONE, nj_mode_search_step_backward);
    bind(context, key_page_down, MDFR_NONE, nj_mode_search_step_forward);
    
    bind(context, 'v', MDFR_CTRL, nj_mode_search_paste);
    
    bind(context, '\n', MDFR_NONE, nj_mode_search_complete);
    bind(context, key_esc, MDFR_NONE, nj_mode_search_cancel);
    end_map(context);
}

CUSTOM_COMMAND_SIG(nj_mode_search_apply)
CUSTOM_DOC("Applies a search query.") {
    uint32_t access = AccessProtected;
    
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t start_pos = nj_mode_search_state.position;
    
    if (nj_mode_search_state.reverse){
        int32_t new_pos = 0;
        buffer_seek_string_insensitive_backward(app, &buffer, start_pos - 1, 0, nj_mode_search_state.bar.string.str, nj_mode_search_state.bar.string.size, &new_pos);
        if (new_pos >= 0){
            if (nj_mode_search_state.step){
                nj_mode_search_state.position = new_pos;
                start_pos = new_pos;
                buffer_seek_string_insensitive_backward(app, &buffer, start_pos - 1, 0, nj_mode_search_state.bar.string.str, nj_mode_search_state.bar.string.size, &new_pos);
                if (new_pos < 0){
                    new_pos = start_pos;
                }
            }
            nj_mode_search_state.match.start = new_pos;
            nj_mode_search_state.match.end = nj_mode_search_state.match.start + nj_mode_search_state.bar.string.size;
        }
    }
    else{
        int32_t new_pos = 0;
        buffer_seek_string_insensitive_forward(app, &buffer, start_pos + 1, 0, nj_mode_search_state.bar.string.str, nj_mode_search_state.bar.string.size, &new_pos);
        if (new_pos < buffer.size){
            if (nj_mode_search_state.step){
                nj_mode_search_state.position = new_pos;
                start_pos = new_pos;
                buffer_seek_string_insensitive_forward(app, &buffer, start_pos + 1, 0, nj_mode_search_state.bar.string.str, nj_mode_search_state.bar.string.size, &new_pos);
                if (new_pos >= buffer.size){
                    new_pos = start_pos;
                }
            }
            nj_mode_search_state.match.start = new_pos;
            nj_mode_search_state.match.end = nj_mode_search_state.match.start + nj_mode_search_state.bar.string.size;
        }
    }
    nj_mode_search_state.step = false;
    
    view_set_highlight(app, &view, nj_mode_search_state.match.start, nj_mode_search_state.match.end, true);
}

CUSTOM_COMMAND_SIG(nj_mode_search_append_char)
CUSTOM_DOC("Appends a char from a search string.") {
    User_Input in = get_command_input(app);
    uint8_t character[4];
    uint32_t length = to_writable_character(in, character);
    
    if (length != 0 && key_is_unmodified(&in.key)){
        append_ss(&nj_mode_search_state.bar.string, make_string(character, length));
        nj_mode_search_apply(app);
    }
}

CUSTOM_COMMAND_SIG(nj_mode_search_backspace_char)
CUSTOM_DOC("Backspaces a char from a search string.") {
    bool32 made_change = backspace_utf8(&nj_mode_search_state.bar.string);
    if(made_change){
        if (nj_mode_search_state.match.end > nj_mode_search_state.match.start + nj_mode_search_state.bar.string.size){
            nj_mode_search_state.match.end = nj_mode_search_state.match.start + nj_mode_search_state.bar.string.size;
        }
        nj_mode_search_apply(app);
    }
}

CUSTOM_COMMAND_SIG(nj_mode_search_step_forward)
CUSTOM_DOC("Moves to the next match in the search.") {
    int32_t start_pos = nj_mode_search_state.position;
    nj_mode_search_state.step = true;
    
    if (nj_mode_search_state.reverse){
        start_pos = nj_mode_search_state.match.start + 1;
        nj_mode_search_state.position = start_pos;
        nj_mode_search_state.reverse = false;
        nj_mode_search_state.step = false;
    }
    
    nj_mode_search_apply(app);
}

CUSTOM_COMMAND_SIG(nj_mode_search_step_backward)
CUSTOM_DOC("Moves to the previous match in the search.") {
    int32_t start_pos = nj_mode_search_state.position;
    nj_mode_search_state.step = true;
    
    if (!nj_mode_search_state.reverse){
        start_pos = nj_mode_search_state.match.start - 1;
        nj_mode_search_state.position = start_pos;
        nj_mode_search_state.reverse = true;
        nj_mode_search_state.step= false;
    }
    
    nj_mode_search_apply(app);
}

CUSTOM_COMMAND_SIG(nj_mode_search_cancel)
CUSTOM_DOC("Seek back to position before starting the search, then activate the previous mode."){
    View_Summary view = get_active_view(app, AccessProtected);
    end_query_bar(app, &nj_mode_search_state.bar, 0);
    view_set_highlight(app, &view, 0, 0, false);
    view_set_cursor(app, &view, seek_pos(nj_mode_search_state.last_position.pos), true);
    nj_finish_chord_action(app, NJ_MODE_MAPID(NJ_CURRENT_MODE));
}

CUSTOM_COMMAND_SIG(nj_mode_search_complete)
CUSTOM_DOC("Seek back to position before starting the search, then activate the previous mode."){
    View_Summary view = get_active_view(app, AccessProtected);
    end_query_bar(app, &nj_mode_search_state.bar, 0);
    view_set_highlight(app, &view, 0, 0, false);
    nj_finish_chord_action(app, NJ_MODE_MAPID(NJ_CURRENT_MODE));
}

CUSTOM_COMMAND_SIG(nj_mode_search_paste)
CUSTOM_DOC("Insert the text at the top of the clipboard to the active search query.") {
    uint32_t access = AccessProtected;
    int32_t count = clipboard_count(app, 0);
    if (count > 0){
        View_Summary view = get_active_view(app, access);
        
        int32_t paste_index = 0;
        view_paste_index[view.view_id].index = paste_index;
        
        int32_t len = clipboard_index(app, 0, paste_index, 0, 0);
        
        if (len <= (NJ_SEARCH_STATE_STRING_SPACE-nj_mode_search_state.bar.string.size)){
            clipboard_index(app, 0, paste_index, (nj_mode_search_state.bar.string.str + nj_mode_search_state.bar.string.size), len);
            nj_mode_search_state.bar.string.size += len;
            nj_mode_search_apply(app);
        }
    }
}

#endif // _MODE_SEARCH_CPP