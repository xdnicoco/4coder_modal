# 4coder Modal Customization Layer
This is my EXPEREMENTAL(!!) customization layer of 4coder super in the journey to a modal future.
I took some elements from emacs, and some elements from vim, and added my own functions and functionality to create a highly customized modal system. Every mode has custom colors to indicate it is activated.

Tested only on 4coder 2.24.

Major modes:
* Normal  mode: Home mode for all the editing commands, the "home" that starts with 4coder.
* Insert  mode: Text insertion mode, remaps all the keys to writing their respective characters.
* Replace mode: Text replacement mode, replaces the text under the cursor. Mostly buggy.

# Installaion
### WARNING: This is my private custom layer, it is not supported by Allen nor by me, and it's purpose is to be educational and to share my customization code as an example. Also, it is not compatible with OS X.
[Support Allen Webster on Patreon](https://www.patreon.com/mr4thdimention/), clone the repository into your 4coder directory and edit 4coder_default_bindings to the following:
```#if !defined(FCODER_DEFAULT_BINDINGS)
#define FCODER_DEFAULT_BINDINGS

#include "4coder_default_include.cpp"
#include "4coder_modal/nj_modal.cpp"
#include "4coder_modal/nj_hooks.cpp"

#if !defined(NO_BINDING)
extern "C" int32_t
get_bindings(void *data, int32_t size){
    Bind_Helper context_ = begin_bind_helper(data, size);
    Bind_Helper *context = &context_;
    
    nj_set_default_hooks(context);
    nj_keys(context);
    
    int32_t result = end_bind_helper(context);
    return(result);
}
#endif //NO_BINDING

#endif //FCODER_DEFAULT_BINDINGS
```

For further information and documentaion check the [4coder documentaion page](http://4coder.net/custom_docs.html).

#### Screenshots
##### Normal colors (for battery efficiency, working in low light)
![Screenshot of the color scheme, normal colors](https://i.imgur.com/kMg5FMD.png)
##### Inverted colors (for working in bright environment)
![Screenshot of the color scheme, inverted colors](https://i.imgur.com/TGFR6Ax.png)
