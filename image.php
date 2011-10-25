<?php
require_once('config.php');
require_once('tartan.inc');
if ($_GET['name']) {
  $name = urldecode($_GET['name']);
  $tartan = new LyzaTartan($name);
  $ok = $tartan->fromXml($name);
  if (isset($_GET['width'])) {
    $tartan->setDynamicScale($_GET['width']);
  }
  $tartan->showImage();
}