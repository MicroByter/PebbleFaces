
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
        var temp_min = Math.round(response.main.temp_min - 273.15);
        var temp_max = Math.round(response.main.temp_max - 273.15);
        var temperature_f = Math.round((temperature * 9) / 5 + 32);
        var temp_min_f = Math.round((temp_min * 9) / 5 + 32);
        var temp_max_f = Math.round((temp_max * 9) / 5 + 32);
        var icon = iconFromWeatherId(response.weather[0].id);
        var city = response.name;
        
        Pebble.sendAppMessage({
          "WEATHER_ICON_KEY":icon,
          "WEATHER_TEMPERATURE_KEY": "" + temperature_f,
          "WEATHER_CITY_KEY":city,
          "WEATHER_TEMP_MIN": "" + temp_min_f,
          "WEATHER_TEMP_MAX": "" + temp_max_f}
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