var initialised = false;
var uri = 'https://dl.dropboxusercontent.com/s/28r6vnuoaw7k0dm/configurator.html';

function appMessageAck(e) {
    console.log("options sent to Pebble successfully");
}

function appMessageNack(e) {
    console.log("options not sent to Pebble: " + e.error.message);
}

//Pebble.addEventListener("ready", function() {
//    initialised = true;
//});

Pebble.addEventListener("showConfiguration", function() {
    var options = JSON.parse(localStorage.getItem('options'));
    console.log("read options: " + JSON.stringify(options));
    console.log("showing configuration");
    if (options !== null) {
        uri = uri+ '?' + 'background=' + encodeURIComponent(options.background);
    }
    Pebble.openURL(uri);
});

Pebble.addEventListener("webviewclosed", function(e) {
    console.log("configuration closed");
    if (e.response !== '') {
        var options = JSON.parse(decodeURIComponent(e.response));
        console.log("storing options: " + JSON.stringify(options));
        localStorage.setItem('options', JSON.stringify(options));
        Pebble.sendAppMessage(options, appMessageAck, appMessageNack);
    } else {
        console.log("no options received");
    }
});








function iconFromWeatherId(weatherId) {
  if (weatherId < 600) {
    return 2;
  } else if (weatherId < 700) {
    return 3;
  } else if (weatherId > 800) {
    return 1;
  } else {
    return 0;
  }
}

function fetchWeather(latitude, longitude) {
  var req = new XMLHttpRequest();
  req.open('GET', "http://api.openweathermap.org/data/2.5/weather?" +
    "lat=" + latitude + "&lon=" + longitude + "&cnt=1", true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        console.log(req.responseText);

        var response = JSON.parse(req.responseText);
        var temperature = Math.round(response.main.temp - 273.15);
        var temperature_f = Math.round((temperature * 9) / 5 + 32);
        var description = response.weather[0].main;
        console.log(temperature_f);
        console.log(description);
        Pebble.sendAppMessage({
          "WEATHER_TEMPERATURE_KEY": temperature_f + "F",
          "WEATHER_DESCRIPTION": description}
        );

      } else {
        console.log("Error");
      }
    }
  }
  req.send(null);
}

function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchWeather(coordinates.latitude, coordinates.longitude);
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
    "WEATHER_CITY_KEY":"Loc Unavailable",
    "WEATHER_TEMPERATURE_KEY":"N/A"
  });
}

var locationOptions = { "timeout": 15000, "maximumAge": 60000 }; 

Pebble.addEventListener("ready", function(e) {
  console.log("connect!" + e.ready);
  locationWatcher = window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
  console.log(e.type);
});

Pebble.addEventListener("appmessage", function(e) {
  window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
  console.log(e.type);
  console.log(e.payload.temperature);
  console.log("message!");
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("webview closed");
  console.log(e.type);
  console.log(e.response);
});