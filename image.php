<?php
require_once('config.php');
require_once('inc/tartan.inc');
if ($_GET['name']) {
  $base = urldecode($_GET['name']);
  $tartan = new LyzaTartan();
  $ok = $tartan->fromXml($base);
  if (isset($_GET['width'])) {
    $tartan->setTargetWidth($_GET['width']);
  }
  $tartan->toImage(FALSE);
}