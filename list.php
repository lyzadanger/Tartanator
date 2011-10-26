<?php
require('config.php');
require('tartan.inc');

$dir = TARTAN_HTML_DIR;
$tartans = array();
$items = array();
$first_letter = NULL;
// Open a known directory, and proceed to read its contents
if (is_dir($dir)) {
    if ($dh = opendir($dir)) {
        while (($file = readdir($dh)) !== false) {
            if (filetype($dir . $file) === 'file' && strpos($file, '.html') !== FALSE) {
              $tartans[] = substr($file, 0, -5);
            }
        }
        closedir($dh);
    }
}

foreach ($tartans as $base) {
  $tartan = new LyzaTartan();
  $tartan->fromXML($base);
  $tartan->writeImage(160); // Create image if it doesn't exist; it should.
  $display_name = ucwords($tartan->name);
  $items[$display_name] = '';
  $letter = substr($display_name, 0, 1);
  if ($letter != $first_letter) {
    $items[$display_name] .= sprintf('<li role="list-divider">%s</li>', $letter);
    $first_letter = $letter;
  }
  $base = $tartan->getBaseName();
  $items[$display_name] .= sprintf('<li><a href="tartans/%s.html"><img src="tartans/images/%s-160.png" alt="%s" />
    <h3>%s</h3></a></li>',
    $base,
    $base,
    $display_name,
    $display_name);
  unset($tartan);
}

ksort($items);
foreach($items as $li) {
  print $li;
}