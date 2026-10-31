package io.github.tombi.port;

import org.libsdl.app.SDLActivity;

/** The game: SDL2's activity, with the launcher's arguments handed to SDL_main (port2/android/loader.c). */
public class TombiActivity extends SDLActivity {
    public static final String EXTRA_ARGS = "io.github.tombi.port.ARGS";

    @Override
    protected String[] getArguments() {
        String[] a = getIntent() != null ? getIntent().getStringArrayExtra(EXTRA_ARGS) : null;
        return a != null ? a : new String[0];
    }

    @Override
    protected void onDestroy() {
        boolean finishing = isFinishing();
        super.onDestroy();
        // the ":game" process cannot start the game twice (the low range is reserved once, in JNI_OnLoad)
        if (finishing) System.exit(0);
    }
}
