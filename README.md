# geocode-weather-sample

This is a simple app that shows how to integrate [pebble-generic-weather](https://github.com/gregoiresage/pebble-generic-weather) and [pebble-geocode-mapquest](https://github.com/Spitemare/pebble-geocode-mapquest).

## Notes

* You must supply a MapQuest API key; if you don't have one [go get one](https://developer.mapquest.com/). The API key should be passed at build time: `CFLAGS="-DGEOCODE_API_KEY=\"myapikey\"" pebble build`.

* While this is a complete, working solution, you probably don't want to copy-paste it without making some changes. For one, it uses [Enamel](https://github.com/gregoiresage/enamel) to handle binding settings. You will want to either work this into your existing Enamel setup or modify the code to use your own settings framework.

* This isn't a pebble package because there's too many implementation dependant details. Really this is a jumping off point. 
