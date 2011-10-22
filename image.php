<?php
require_once('config.php');
require_once('tartan.inc');
if ($_GET['name']) {
  $name = LyzaTartan::sanitize(urldecode($_GET['name']));
  $tartan = new LyzaTartan($name);
  $filename = $tartan->get_filename();
  $ok = $tartan->from_xml($filename);
  if (isset($_GET['width'])) {
    $tartan->set_dynamic_scale($_GET['width']);
  }
  $tartan->showImage();
}