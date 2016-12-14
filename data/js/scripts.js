



// var urlBase = "http://192.168.1.24/"; // used when hosting the site somewhere other than the ESP8266 (handy for testing without waiting forever to upload to SPIFFS)
var urlBase = ""; // used when hosting the site on the ESP8266

var brightnessTimer = {};
var hueSpeedTimer = {};
var paletteSpeedTimer = {};
var colorTimer = {};

var ignoreColorChange = true;

var allData;
var colors = [
  {l: 'Red', c: '#ff0000'},
  {l: 'Orange', c: '#FF8000'},
  {l: 'Yellow', c: '#FFFF00'},
  {l: 'Chartreuse', c: '#80FF00'},
  {l: 'Green', c: '#00FF00'},
  {l: 'Spring Green', c: '#00FF80'},
  {l: 'Cyan', c: '#00FFFF'},
  {l: 'Azure', c: '#0080FF'},
  {l: 'Blue', c: '#0000FF'},
  {l: 'Violet', c: '#8000FF'},
  {l: 'Magenta', c: '#FF00FF'},
  {l: 'Rose', c: '#FF0080'}
];
var cnt = $('#colors');
var div;
colors.forEach(function(itm) {
  cnt.append('<div class="btn-group" role="group"><button type="button" class="btn btn-default btn-color" style="background: '+itm.c+';" title="'+itm.l+'">&nbsp;</button></div>');
});

$(document).ready( function() {

  $("#inputColor").minicolors(
    {
      theme: "bootstrap",
      changeDelay: 200,
      control: "wheel",
      format: "rgb",
      inline: true
    });

  getAll();
});

$("#btnRefresh").click(function() {
  getAll();
});

$("#btnPowerOn").click(function() {
  setPower(1);
});

$("#btnPowerOff").click(function() {
  setPower(0);
});

$("#inputBrightness").on("change mousemove", function() {
   $("#spanBrightness").html($(this).val());
});

$("#inputBrightness").on("change", function() {
   $("#spanBrightness").html($(this).val());
   delaySetBrightness();
});

$("#inputSpeed").on("change mousemove", function() {
   $("#spanSpeed").html($(this).val());
});

$("#inputHueSpeed").on("change", function() {
   $("#spanHueSpeed").html($(this).val());
   delaySetHueSpeed();
});

$("#inputPaletteSpeed").on("change", function() {
   $("#spanPaletteSpeed").html($(this).val());
   delaySetPaletteSpeed();
});

$("#inputPattern").change(function() {
   setPattern($("#inputPattern option:selected").index());
});

$("#inputColor").change(function() {
  if(ignoreColorChange) return;

  var rgb = $("#inputColor").minicolors('rgbObject');
  // delaySetColor(rgb);
  var hexString = rgbToHex(rgb.r, rgb.g, rgb.b);
  delaySetColor('hex', hexString);

});

$(".btn-color").click(function() {
  // console.log('A')
  if(ignoreColorChange) return;
  // console.log('B')

  var rgb = $(this).css('backgroundColor');
  var components = rgbToComponents(rgb);
  // delaySetColor('rgb', components);

  var hexString = rgbToHex(components.r, components.g, components.b);
  delaySetColor('hex', hexString);

  ignoreColorChange = true;
  $("#inputColor").minicolors('value', hexString);
  ignoreColorChange = false;
});

function getAll() {
  $.get(urlBase + "all", function(data) {
    allData = data;

    $("#status").html("Connecting...");
    $("#inputBrightness").val(data.brightness);
    $("#spanBrightness").html(data.brightness);
    if (data.hueSpeed) $("#inputHueSpeed").val(data.hueSpeed);
    else console.log('no hueSpeed')
    if (data.paletteSpeed) $("#inputPaletteSpeed").val(data.paletteSpeed);
    else console.log('no paletteSpeed')

    var hexString = rgbToHex(data.solidColor.r, data.solidColor.g, data.solidColor.b);
    ignoreColorChange = true;
    $("#inputColor").minicolors('value', hexString);
    ignoreColorChange = false;

    updatePowerButtons(data.power);

    $("#inputPattern").find("option").remove();

    for(var i = 0; i < data.patterns.length; i++) {
      var pattern = data.patterns[i];
      $("#inputPattern").append("<option value='" + i + "'>" + pattern + "</option>");
    }

    $("#inputPattern").val(data.currentPattern.index);
    processPattern(data.currentPattern.index);

    $("#status").html("Ready");
  });
}

function updatePowerButtons(value) {
  if(value == 0) {
    $("#btnPowerOn").attr("class", "btn btn-default");
    $("#btnPowerOff").attr("class", "btn btn-primary");
  } else {
    $("#btnPowerOn").attr("class", "btn btn-primary");
    $("#btnPowerOff").attr("class", "btn btn-default");
  }
}

function setPower(value) {
  $.post(urlBase + "power?value=" + value, function(data) {
    updatePowerButtons(data);
    $("#status").html("Set Power: " + data);
  });
}

function delaySetBrightness() {
    clearTimeout(brightnessTimer);
    brightnessTimer = setTimeout(function() {
      setBrightness($("#inputBrightness").val());
    }, 300);
}

function setBrightness(value) {
  $.post(urlBase + "brightness?value=" + value, function(data) {
    $("#status").html("Set Brightness: " + data);
  });
}

function delaySetHueSpeed() {
    clearTimeout(hueSpeedTimer);
    hueSpeedTimer = setTimeout(function() {
      setHueSpeed($("#inputHueSpeed").val());
    }, 300);
}
function delaySetPaletteSpeed() {
    clearTimeout(paletteSpeedTimer);
    paletteSpeedTimer = setTimeout(function() {
      setPaletteSpeed($("#inputPaletteSpeed").val());
    }, 300);
}

function setHueSpeed(value) {
  $.post(urlBase + "speed?type=hue&value=" + value, function(data) {
    $("#status").html("Set Hue Speed: " + data);
  });
}

function setPaletteSpeed(value) {
  $.post(urlBase + "speed?type=palette&value=" + value, function(data) {
    $("#status").html("Set Palette Speed: " + data);
  });
}

function processPattern(p) {
  if (p == 0) {
    $("#wrapperHueSpeed").css('display', 'none');
    $("#wrapperPaletteSpeed").css('display', 'none');
    $("#wrapperColorWheel").css('display', 'inherit');
    $("#wrapperColorPicker").css('display', 'inherit');
    // $("#wrapperPaletteSpeed").styles.display = 'none';
    // $("#wrapperColorWheel").styles.display = 'inherit';
    // $("#wrapperColorPicker").styles.display = 'inherit';
  } else {
    $("#wrapperHueSpeed").css('display', 'inherit');
    $("#wrapperPaletteSpeed").css('display', 'inherit');
    $("#wrapperColorWheel").css('display', 'none');
    $("#wrapperColorPicker").css('display', 'none');
  }
}
function setPattern(value) {
  $.post(urlBase + "pattern?value=" + value, function(data) {
    processPattern(value);
    $("#status").html("Set Pattern: " + data.name);
  });
}

function delaySetColor(colorType, value) {
  clearTimeout(colorTimer);
  colorTimer = setTimeout(function() {
    setColor(colorType, value);
  }, 300);
}

function setColor(colorType, value) {
  // console.log('set', colorType, value)
  if (colorType == "rgb") {
    $.post(urlBase + "solidColor?r=" + value.r + "&g=" + value.g + "&b=" + value.b, function(data) {
    $("#status").html("Set Color: rgb(" + data.r + ", " + data.g + ", " + data.b + ")");
    });
 } else {
    $.post(urlBase + "color?value=" + value.replace('#', ''), function(data) {
      $("#status").html("Set Color: hex(" + value + ")");
    });
  }
  $("#inputPattern").val(0);
}

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function rgbToComponents(rgb){
  var components = {};

   rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
   components.r = parseInt(rgb[1]);
   components.g = parseInt(rgb[2]);
   components.b = parseInt(rgb[3]);

   return components;
 }
