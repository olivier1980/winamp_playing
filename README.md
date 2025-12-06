Based on https://github.com/clandrew/wdrp/tree/master/DiscordRichPresence

Simple Winamp plugin that works under Wine.

Writes the current playing song to /tmp/oli_current_playing.txt.

Now this file can be read by other programs in Linux and other actions can be taken.

In my case, I want a quick shortcut to delete the currently playing song, and existing Winamp "delete file" plugins weren't working under Wine.
For that, I've included `rm_now_playing`, a bash script that can be trigger by a keypress for example.

It will convert the windows filename to unix format, delete the file and push a notification to Ubuntu.

The only thing that I haven't automated yet is Winamp skipping to the next track after delete, and removing the file from playlist.