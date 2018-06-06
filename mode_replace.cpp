/*
This mode replaces the character under the cursor with the characters pressed.
This whole mode is a hack, very buggy and in general should not be used yet.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_REPLACE_CPP)
#define _MODE_REPLACE_CPP

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x0f0515, // color_bg
                             0x311b92, // color_bar
                             0x332ba2, // color_bar_hover
                             0x353bb2, // color_bar_active
                             0xe91e63, // color_mode
                             0xc30c4a, // color_mark
                             0x030c7f, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_replace)
CUSTOM_DOC("Activates 'replace' mode.")
{
    NJ_ENTER_MODE(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    bind_vanilla_keys(context, nj_replace_character);
    bind(context, ' ', MDFR_SHIFT, write_character);
    bind(context, 't', MDFR_NONE, word_complete);
    
    bind(context, key_insert, MDFR_NONE, nj_activate_previous_mode);
    bind(context, key_back,   MDFR_NONE, nj_replace_mode_backspace);
    
    bind(context, 'x', MDFR_CTRL, nj_replace_mode_cut);
    bind(context, 'X', MDFR_CTRL, nj_replace_mode_cut_line);
    bind(context, 'x', MDFR_ALT,  nj_replace_mode_cut_token_or_word);
    
    bind(context, 'e', MDFR_ALT,  nj_replace_mode_snipe_token_or_word);
    
    bind(context, 'v', MDFR_CTRL, nj_replace_mode_paste);
    bind(context, 'V', MDFR_CTRL, nj_replace_mode_paste_next);
    bind(context, 'p', MDFR_CTRL, nj_replace_mode_paste);
    bind(context, 'P', MDFR_CTRL, nj_replace_mode_paste_next);
    
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    inherit_map(context, mapid_movements);
    end_map(context); //mapid_replace
}

/*
The following command is stolen from ChronalDragon's 4vim.
   For more details check https://github.com/Chronister/4vim/
*/
CUSTOM_COMMAND_SIG(nj_replace_character)
CUSTOM_DOC("Replaces the character under the cursor with whatever character was used to trigger this command."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    if(buffer_get_char(app, &buffer, view.cursor.pos) != '\n') {
        delete_char(app);
    }
    write_character(app);
}

// HACK(NJ): You should be more smart than just undoing twice
CUSTOM_COMMAND_SIG(nj_replace_mode_backspace)
CUSTOM_DOC("Undoes the last two operation, then moves to the left. HACKIE AS HELL."){
    undo(app);
    undo(app);
    move_left(app);
}

static int32_t
nj_replace_mode_delete(Application_Links *app, int32_t start, int32_t end, Buffer_Summary *buffer_out){
    Buffer_Summary buffer = {0};
    int32_t result = false;
    
    int32_t size = (end - start);
    if (size <= app->memory_size){
        char *str = (char*)app->memory;
        for(int i = 0; i < size; str[++i] = ' ');
        
        buffer_replace_range(app, &buffer, start, end, str, size);
        if (buffer_out){*buffer_out = buffer;}
        result = 1;
    }
    
    return(result);
}

static int32_t
nj_replace_mode_clipboard_cut(Application_Links *app, int32_t start, int32_t end, Buffer_Summary *buffer_out, uint32_t access){
    Buffer_Summary buffer = {0};
    int32_t result = false;
    
    if (post_buffer_range_to_clipboard(app, &global_part, 0, &buffer, start, end)){
        nj_replace_mode_delete(app, start, end, &buffer);
        if (buffer_out){*buffer_out = buffer;}
    }
    
    return(result);
}

CUSTOM_COMMAND_SIG(nj_replace_mode_snipe_token_or_word)
CUSTOM_DOC("Replaces a single, whole token on or to the left of the cursor with spaces."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t start = buffer_boundary_seek(app, &buffer, view.cursor.pos, 0, BoundaryToken | BoundaryWhitespace);
    int32_t end   = buffer_boundary_seek(app, &buffer, start,           1, BoundaryToken | BoundaryWhitespace);
    
    nj_replace_mode_delete(app, start, end, 0);
}

CUSTOM_COMMAND_SIG(nj_replace_mode_cut_token_or_word)
CUSTOM_DOC("Copies a single, whole token on or to the left of the cursor, then replaces it with spaces."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t start = buffer_boundary_seek(app, &buffer, view.cursor.pos, 0, BoundaryToken | BoundaryWhitespace);
    int32_t end   = buffer_boundary_seek(app, &buffer, start,           1, BoundaryToken | BoundaryWhitespace);
    
    nj_replace_mode_clipboard_cut(app, start, end, 0, access);
}

CUSTOM_COMMAND_SIG(nj_replace_mode_cut)
CUSTOM_DOC("Copies the range to clipboard, then replaces it with spaces"){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Range range = get_view_range(&view);
    nj_replace_mode_clipboard_cut(app, range.min, range.max, 0, access);
}

CUSTOM_COMMAND_SIG(nj_replace_mode_cut_line)
CUSTOM_DOC("Copies the line under the cursor to clipboard, then replaces it with spaces"){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    int32_t start = seek_line_beginning(app, &buffer, view.cursor.pos);
    int32_t end = seek_line_end(app, &buffer, view.cursor.pos) + 1;
    nj_replace_mode_clipboard_cut(app, start, end, 0, access);
}

CUSTOM_COMMAND_SIG(nj_replace_mode_paste)
CUSTOM_DOC("At the cursor, replace the text to the right of the cursor with the text at the top of the clipboard."){
    int32_t count = clipboard_count(app, 0);
    
    if (count > 0){
        uint32_t access = AccessOpen;
        View_Summary view = get_active_view(app, access);
        
        view_paste_index[view.view_id].next_rewrite = RewritePaste;
        
        int32_t paste_index = 0;
        view_paste_index[view.view_id].index = paste_index;
        
        int32_t len = clipboard_index(app, 0, paste_index, 0, 0);
        char *str = 0;
        
        if (len <= app->memory_size){
            str = (char*)app->memory;
        }
        
        if (str){
            clipboard_index(app, 0, paste_index, str, len);
            
            Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
            int32_t pos = view.cursor.pos;
            buffer_replace_range(app, &buffer, pos, pos+len, str, len);
            view_set_mark(app, &view, seek_pos(pos));
            view_set_cursor(app, &view, seek_pos(pos + len), true);
            
            // TODO(allen): Send this to all views.
            Theme_Color paste;
            paste.tag = Stag_Paste;
            get_theme_colors(app, &paste, 1);
            view_post_fade(app, &view, 0.667f, pos, pos + len, paste.color);
        }
    }
}

CUSTOM_COMMAND_SIG(nj_replace_mode_paste_next)
CUSTOM_DOC("TODO"){
    int32_t count = clipboard_count(app, 0);
    
    if (count > 0){
        uint32_t access = AccessOpen;
        View_Summary view = get_active_view(app, access);
        
        if (view_paste_index[view.view_id].rewrite == RewritePaste){
            view_paste_index[view.view_id].next_rewrite = RewritePaste;
            
            int32_t paste_index = view_paste_index[view.view_id].index + 1;
            view_paste_index[view.view_id].index = paste_index;
            
            int32_t len = clipboard_index(app, 0, paste_index, 0, 0);
            char *str = 0;
            
            if (len <= app->memory_size){
                str = (char*)app->memory;
            }
            
            if (str){
                clipboard_index(app, 0, paste_index, str, len);
                
                Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
                Range range = get_view_range(&view);
                int32_t pos = range.min;
                
                undo(app);
                buffer_replace_range(app, &buffer, range.min, range.max, str, len);
                view_set_cursor(app, &view, seek_pos(pos-1), true);
                
                // TODO(allen): Send this to all views.
                Theme_Color paste;
                paste.tag = Stag_Paste;
                get_theme_colors(app, &paste, 1);
                view_post_fade(app, &view, 0.667f, pos, pos + len, paste.color);
            }
        }
        else{
            nj_replace_mode_paste(app);
        }
    }
}

#endif // _MODE_REPLACE_CPP