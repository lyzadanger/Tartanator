<!DOCTYPE html> 
<html> 
  <head> 
  <title>The Tartanator</title> 
  
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1"> 
  <link rel="stylesheet" href="css/jquery.mobile-1.0rc1.min.css" />
  <link rel="stylesheet" href="css/styles.css" />

  <script src="js/jquery-1.6.4.min.js"></script>
  <script type="text/javascript">
    // Need to bind to mobileinit before jQ mobile library is loaded
    $(document).bind('mobileinit',function(){
      $.mobile.selectmenu.prototype.options.nativeMenu = false;
    });
  </script>
  <script src="js/jquery.mobile-1.0rc1.min.js"></script>

</head> 
<body> 

  <div data-role="page" id="tartan-maker">

    <div data-role="header" data-position="fixed">
      <h1>Tartan Builder</h1>
    </div><!-- /header -->

    <div data-role="content">  
      <script src="js/tartanator.js"></script>
      <form method="post" action="generate.php" id="tartanator_form">
        <ul data-role="listview" id="tartanator_form_list">
          <li data-role="list-divider">Tell us about your tartan</li>
          <li data-role="fieldcontain">
            <label for="tartan_name">Tartan Name</label>
            <input type="text" name="name" id="tartan_name" placeholder="Tartan Name" />
          </li>
          <li data-role="fieldcontain">
            <label for="tartan_info">Tartan Info</label>
            <textarea cols="40" rows="8" name="tartan_info" id="tartan_info" placeholder="Optional tartan description or info"></textarea>
          </li>
          <li data-role="list-divider">Build your colors</li>
      
          <?php for ($i = 0; $i < 6; $i++): // 6 color fields ?>
            <li class="colorset">
              <div data-role="fieldcontain" class="color-input">
              <label class="select" for="color-<?php print $i ?>" data-native-menu="false">Color</label>
                <select name="colors[]" id="color-<?php print $i ?>" >
                  <option value="">Select a Color</option>
                  <option value="#000000">Black</option>
                  <option value="#ffffff">White</option>
                  <option value="#cccccc">Light Grey</option>
                  <option value="#999999">Mid Grey</option>
                  <option value="#666666">Dark Grey</option>
                  <option value="#333333">Very Dark Grey</option>
                  <option value="#cc0000">Red</option>
                  <option value="#660000">Dark Red</option>
                  <option value="#FFB6C1">Light Rose</option>
                  <option value="#ff3344">Rose</option>
                  <option value="#FF8C00">Orange</option>
                  <option value="#FFD700">Gold</option>            
                  <option value="#ffec00">Yellow</option>
                  <option value="#9ACD32">Yellow Green</option>
                  <option value="#5b6333">Olive</option>
                  <option value="#00cc00">Light Green</option>
                  <option value="#546c18">Field Green</option>
                  <option value="#8FBC8F">Light Grey Green</option>
                  <option value="#008000">Green</option>
                  <option value="#126846">Blue Green</option>
                  <option value="#B0E0E6">Light Blue</option>
                  <option value="#274086">Blue</option>
                  <option value="#56565e">Storm Blue Grey</option>
                  <option value="#3c516c">Dark Slate Blue</option>
                  <option value="#001144">Dark Blue</option>
                  <option value="#29292b">Very Dark Blue</option>
                  <option value="#4B0082">Indigo</option>
                  <option value="#8A2BE2">Blue Violet</option>
                  <option value="#65295f">Purple</option>
                  <option value="#cc9966">Light Tan</option>
                  <option value="#996600">Tan</option>
                  <option value="#615024">Welsh Earth</option>
                  <option value="#663300">Brown</option>
                </select>
              </div>
          
              <div data-role="fieldcontain" class="size-input">
                <label for="size-<?php print $i ?>">Stitch Count</label>
                <input id="size-<?php print $i ?>" type="range" min="2" step="2" max="72" autocomplete="off" name="sizes[]" value="2" />
                </div>
            </li>
          <?php endfor; ?>
        </ul>

        <ul data-role="listview" data-inset="true" id="colorlist">
        </ul>

        <div data-role="fieldcontain">
          <input type="submit" name="buildtartan" id="buildtartan" value="Make it!" data-role="button" />
        </div>

      </form>

    </div><!-- /content -->

  </div><!-- /page -->

</body>
</html>