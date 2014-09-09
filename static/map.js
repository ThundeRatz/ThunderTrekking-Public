var map;
var marker = [];

function initialize() {
  var marcadores = [new google.maps.LatLng(-23.555844,-46.730087),
                    new google.maps.LatLng(-23.556115,-46.729843),
                    new google.maps.LatLng(-23.556203,-46.730269)];
  var mapOptions = {
    center: new google.maps.LatLng(-23.556014,-46.730135),
    disableDefaultUI: false,
    zoom: 19,
    mapTypeId: google.maps.MapTypeId.SATELLITE
  };
  
  map = new google.maps.Map(document.getElementById("map_canvas"), mapOptions);
  
  for (var i = 0; i < marcadores.length; i++) {
    marker.push(new google.maps.Marker({
      position: marcadores[i],
      map: map,
      title: 'Ponto ' + i
    }));
  }
}

function update_map(lat, lon, ponto) {
  alert(typeof marker[ponto]);
  marker[ponto].setMap(null);
  marker[ponto] = null;
  marker[ponto] = new google.maps.Marker({
    position: new google.maps.LatLng(lat, lon),
    map: map,
    title: 'Ponto ' + ponto
  });
}
