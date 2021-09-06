# <strong>GUMBO editor</strong>

Simple editor built upon [kilo](https://github.com/antirez/kilo) editor. Still big work in progress although this is
just fun side project to learn more C/C++. 

From 0.0.2->Alpha Switching to ncurses. Added handling window creation, key strokes and window refresh.

# Features

For now all you can do is edit text and look at text as you want.
You can copy and paste text as you please as you can now mark and copy lines as you please, terminal is muc more open than it was since ncurses handles that better.
Also editor now can highlight python syntax.

# Compile and run

Make sure you have <strong>g++ and cmake</strong> installed on your system and all you need to do is run
`build.sh` script and it will build everything into `build` folder. To run it all you need to do is to navigate
to `cd build/src` and run it `./gum <filename>` or just `./gum` as it does not need file argument to
run or simply run it by typing `./build/src/gum <filename>` or `/build/src/gum`.
