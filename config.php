<?php
if (file_exists('config.inc')) {
  require_once('config.inc');
} else {
  // paths relative to DOCROOT
  define('TARTAN_DOC_ROOT', '/' .  ltrim(substr(dirname(__FILE__), strlen($_SERVER['DOCUMENT_ROOT'])), '/'));
  define('PUBLIC_TARTAN_DIR', rtrim(TARTAN_DOC_ROOT, '/') . '/tartans/');
  // Absolute filesystem paths
  define('TARTAN_XML_DIR', dirname(__FILE__) . '/tartans/data/');
  define('TARTAN_IMAGE_DIR', dirname(__FILE__) . '/tartans/images/');
  define('TARTAN_HTML_DIR', dirname(__FILE__) . '/tartans/');
}