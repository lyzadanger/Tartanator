# Tartanator

## Configuration
This code should in theory be config-less. It should be able to determine where it is installed relative to your Web server's `DOCUMENT_ROOT`. However, should you wish to alter this or the directories where tartan data, HTML and images are written, you can copy `config.inc.example` to `config.inc` and edit the settings therein.

## jQuery Mobile Images

There's an `images` directory inside `css` — this is intended for the jQuery mobile images only, since that's how their paths are set up.