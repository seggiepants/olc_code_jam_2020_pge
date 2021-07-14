# The Great Machine Armada
by: seggiepants

OLC Pixel Game Engine 2 Port
Ported: 7/6/2021 - 7/13/2021

![Screenshot](https://raw.githubusercontent.com/seggiepants/olc_code_jam_2020_pge/master/screenshot.png)
<https://github.com/seggiepants/olc_code_jam_2020pge>

This is a port of my entry for the OLC Code Jam 2020 to the Pixel Game Engine 2.0 from SDL2.

## Controls:
* A or Left Arrow - Move the tank left
* D or Right Arrow - Move the tank right
* Space Bar - Fire the tank's canon.
* P - Pause the game
* ESCAPE - Exit to the title (will also exit to the from the main menu OS in the windows version)
* I am not getting joystick support to work, but it is supposed to.

## Technology:
I have not yet ported this to use emscripten as the SDL2 version does. I also don't have gamepad support working, unless windows just doesn't like my gamepad.
Ordered a new joystick, will see if that helps when it arrives.

This project still uses several libraries/resources/programs including the following:
* OLC Pixel Game Engine 2.0, game library <https://github.com/OneLoneCoder/olcPixelGameEngine>
* OLC Pixel Game Engine 2.0 Extension olcPGEX_Sound <https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Extensions/olcPGEX_Sound.h>
* OLC Pixel Game Engine 2.0 Extension olcPGEX_Gamepad <https://github.com/gorbit99/olcPGEX_Gamepad>
* OLC Pixel Game Engine 2.0 ImGui connector <https://github.com/dandistine/olcPGEDearImGui>
* Dear ImGui - Immediate mode GUI library <https://github.com/ocornut/imgui>
* Dear ImGui OpenGL 2 BackEnd <https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_opengl2.h>
* Nova Font - Used with SDL TTF <http://fontlibrary.org>
* Images from Kenney.nl <https://kenney.nl/>
* BFXR (standalone) for sound effect generation <https://www.bfxr.net/>
* GIMP for image editing <https://www.gimp.org/>
* Visual Studio 2019 Community Edition <https://visualstudio.microsoft.com/>
* Notepad++ <https://notepad-plus-plus.org/>

## Compilation:
### Windows:
You should be able to load the solution file from the repository. You will need to update include and library paths to where SDL2 is located on your machine. I did combine all of the various SDL 2 libraries together into a folder, if you did not you many need to add extra folders. Otherwise it should compile normally in debug and release x64.

### WebAssembly
Not attempted yet.

## Problems
Still haven't fixed the folders where things should go.

Way more add-ons than I was expecting.

## Thoughts
Could probably use some newer/better C++ idoms.

I was surprised how nice ImGui worked once integrated. It took quite a while to get working but then things like the next level, and game over popups went surprisingly easy. If I had more time there would have been a lot more menu options. The immediate mode GUI aspect still puzzles me in places. Again, I need more time to look at it.

FreeLib2 library reference took me way longer to get working than it should have. You need to set the header files as default folders so you can use #include <...>

I want to eventually merge the two versions together as well as an SDL 1 version, from there I can start on a Game Jam 2d framework.