<?php require_once('config.php'); ?>
<!DOCTYPE html> 
<html> 
	<head> 
	<title>The Tartanator</title> 
	
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
  <link rel="stylesheet" href="css/jquery.mobile-1.0rc1.min.css" />
  <script src="js/jquery-1.6.4.min.js"></script>
  <script src="js/jquery.mobile-1.0rc1.min.js"></script>
</head> 
<body> 

<div data-role="page">
	<div data-role="header">
    <a href="index.php" data-role="button" data-icon="plus" class="ui-btn-right" data-theme="b">Create</a>
		<h1>Tartans!</h1>
	</div><!-- /header -->

	<div data-role="content">
    <ul data-role="listview" id="tartan-list">
      <?php include('inc/list.php'); ?>
    </ul>
	</div><!-- /content -->

</div><!-- /page -->

</body>
</html>