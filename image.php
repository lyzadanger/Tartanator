<?php
require_once('config.php');
require_once('tartan.inc');
if ($_GET['name']) {
  $name = LyzaTartan::sanitize(urldecode($_GET['name']));
  $tartan = new LyzaTartan($name);
  $filename = $tartan->getFilename();
  $ok = $tartan->fromXml($filename);
  if (isset($_GET['width'])) {
    $tartan->setDynamicScale($_GET['width']);
  }
  $tartan->showImage();
}