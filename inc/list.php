<?php
/*
 * Generate <li> elements for tartans dynamically, based on the HTML
 * files present in the tartans HTML directory.
 */
require_once(dirname(__FILE__) . '/../config.php');
require_once(dirname(__FILE__) . '/tartan.inc');

$dir        = TARTAN_HTML_DIR;
$tartans    = array();
$list_items = array();

// Iterate over tartans HTML directory and find .html files
// Build $tartans array with this info
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
  $tartan                     = new LyzaTartan();
  $tartan->fromXML($base);
  $base                       = $tartan->getBaseName();
  $display_name               = ucwords($tartan->name);
  $list_items[$display_name]  = '';
  // Munge together the HTML output
  $tartan_image = '';
  if (file_exists(TARTAN_IMAGE_DIR . $base . '-200.png')) {
    // Generate image tag if image exists
    $tartan_image = sprintf('<img src="%simages/%s-200.png" alt="%s" />',
      PUBLIC_TARTAN_DIR,
      $base,
      $display_name);
  }
  $list_items[$display_name] .= sprintf('<li id="tartan-%s"><a href="%s%s.html">%s<h3>%s</h3></a></li>',
    $base,
    PUBLIC_TARTAN_DIR,
    $base,
    $tartan_image,
    $display_name);
  unset($tartan);
}
ksort($list_items); // Sort array of tartan HTML <li> elements by key (display name)

// Track the current first letter of the tartans we're listing
$current_letter = '';
foreach($list_items as $display_name => $li) {
  $first_letter = substr($display_name, 0, 1);
  if ($first_letter != $current_letter) {
    // If the first letter of this tartan is different than
    // the first letter of the preceding tartan, create a list
    // divider with the new letter
    printf('<li role="list-divider">%s</li>', $first_letter);
    $current_letter = $first_letter;
  }
  print $li; // Print each <li> in the array of tartans
}
?>