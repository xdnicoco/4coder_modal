/*
This mode inserts the next character pressed after activating it,
then returns to the previous mode.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_CHORD_INSERT_SINGLE_CPP)
#define _MODE_CHORD_INSERT_SINGLE_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x050f15, // color_bg
                             0x032459, // color_bar
                             0x021f4f, // color_bar_hover
                             0xb74430, // color_bar_active
                             0xe95420, // color_mode
                             0x8d360d, // color_mark
                             0x030c7f, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_insert_single)
CUSTOM_DOC("Activates 'chord insert single' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind_vanilla_keys(context, nj_insert_character_then_prev);
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    end_map(context);
}

CUSTOM_COMMAND_SIG(nj_insert_character_then_prev)
CUSTOM_DOC("Inserts whatever character was used to trigger this command, then return to the previous mode."){
    exec_command(app, write_character);
    exec_command(app, nj_activate_previous_mode);
}

#endif // _MODE_CHORD_INSERT_SINGLE_CPP