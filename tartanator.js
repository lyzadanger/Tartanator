(function () {

  var $page, $colorList, $submitFormBtn, $sizeInputUI, $colorInputUI, $nameInput;

  $page = $('[data-role="page"]');

  // Triggered when the page has been created in the DOM (via ajax or other) 
  // but before all widgets have had an opportunity to enhance the contained markup. 
  // - We'll only show one set of inputs and handle others with the "add color" button
  $page.live('pagecreate', function () {
    jQuery.ajaxSettings.traditional = true;
    $('.colorset:not(:first)').remove();
    buildAddButton();
  });

  // Triggered after jQuery Mobile DOM elements have been initialized
  $page.live('pageinit', init);

  function init () {
    $colorList     = $('#colorlist');
    $submitFormBtn = $('#buildtartan').parents('.ui-btn');
    $sizeInputUI   = $('.size-input');
    $sizeSlider    = $sizeInputUI.find('input');
    $colorInputUI  = $('.color-input');
    $nameInput     = $('#tartan_name');

    $submitFormBtn.add('.color-input label').hide();
    $('#color-0 option').each(styleColorListItem);
    setColorSelectStyle();

    $colorList.click(onColorListChange);
    $sizeSlider.change(onStitchSizeChange);
    $colorInputUI.change(onColorSelectChange);
    $('#tartanator_form').submit(onFormSubmit);
  }

  // Add a color indication to each line of the custom jQM select menu
  function styleColorListItem (index, value) {
    var hex    = $(this).val();
    if (hex) {
      $('#color-0-menu li a').eq(index).css('borderLeft', '25px solid ' + hex);
    }
  }

  // Create and add the add-color button to the DOM
  function buildAddButton() {
    var li = $('<li></li>').attr({
      'data-role' : 'fieldcontain',
      'id'        : 'add-color-container'
    });
    var button = $('<input type="button">').attr({
      'name'      : 'addcolor',
      'data-role' : 'button',
      'value'     : 'Add This Color',
      'data-icon' : 'plus'
    });
    button.click(onAddColor);
    $('#tartanator_form_list').append(li.append(button));
  }

  // User clicks the "add color" button
  function onAddColor (evt) {
    var form   = $(this).closest('form'),
        select = form.find('select'),
        name   = select.find(':selected').text(),
        hex    = select.val(),
        size   = form.find('.size-input input').val();
    if (hex && size) {
      addColor(name, size, hex);
      onColorListChange();
    } else {
     $.mobile.changePage( "dialogs/size-color-required.html", {
       transition: "pop",
       reverse: false,
       role: 'dialog'
      });	
    }
    return false;
  };

  // Upate the DOM with the new color
  // Create and add hidden fields to contain values for the form
  function addColor (colorName, colorSize, colorValue) {
    var colorItem = [
      '<li data-role="button" data-icon="delete" style="background:', colorValue,
      '; color:', (isDarkColor(colorValue) ? '#fff' : '#000'),
      '">', colorName, ' (', colorSize + ')',
      '<input type="hidden" name="colors[]" value="', colorValue, '">',
      '<input type="hidden" name="sizes[]" value="', colorSize, '">',
      '<a data-role="button"></a>',
      '</li>'].join('');
    $colorList.append(colorItem);
    $colorInputUI.find('select').val('').selectmenu('refresh');
    $sizeInputUI.find('input').slider('refresh');
    onColorListChange();
  };

  // Called when the list of added colors changes
  // e.g., when adding a new color or deleting an existing one
  function onColorListChange (deleteClickEvent) {
    var $li;
    if (deleteClickEvent) $li = $(deleteClickEvent.target).closest('li').remove();
    $submitFormBtn[$colorList.find('li').length ? 'show' : 'hide']();
    $colorList.listview('refresh'); 
    setColorSelectStyle();
  };
  
  // User has changed the value of the stich size; only allow even values
  function onStitchSizeChange(changeEvent) {
    var current_val = parseInt($sizeSlider.val(), 10);
    if (current_val % 2 == 1) {
      $sizeSlider.val(current_val + 1);
    }
  }

  function onFormSubmit () {
    var url;
    if (!$nameInput.val() || !$colorList.find('li').length) {
     $.mobile.changePage( "dialogs/tartan-data-required.html", {
       transition: "pop",
       reverse: false,
       role: 'dialog'
      });	
      return false;
    }
    url = $(this).attr('action');
    // Append the name as a GET param as well, to support
    // re-rendering the page when user hits the forward button
    $(this).attr('action', url + '?name=' + $nameInput.val() + '&width=300');
    $('[name=redirect_to_image]').val('false');
    return true;
  };
  
  function onColorSelectChange() {
    setColorSelectStyle();
    $('#size-0').focus();
  }

  // Set the background color of the select widget to match the color value
  function setColorSelectStyle() {
    var backgroundHex = $colorInputUI.find('select').val();
    $colorInputUI.find('.ui-btn').css({
      'background': backgroundHex || '',
      'color'     : isDarkColor(backgroundHex) ? '#fff' : '#000'
    });
  };

  // Given a (string) hex value, do a quick & dirty calculation to indicate whether 
  // black or white should be used as a contrast color
  function isDarkColor (hex) {
    var sum;
    if (!hex) return false;
    hex = (hex + '').match(/[^#]+/)[0];
    if (hex.length == 3) hex += hex;
    sum = parseInt(hex.substr(0,2), 16) + parseInt(hex.substr(2,2), 16) + parseInt(hex.substr(4,2), 16);
    return (sum / 3) < 128;
  };

}());