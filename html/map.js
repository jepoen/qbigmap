String.prototype.contains = function(it) {
  return this.indexOf(it) != -1;
};

// Icons
var icoInfo = L.icon({
  iconUrl: 'ico/flag.png',
  iconSize: [32, 37],
  iconAnchor: [16, 37],
  popupAnchor: [0, -37],
});

var icoStar = L.icon({
  iconUrl: 'ico/star-3.png',
  iconSize: [32, 37],
  iconAnchor: [16, 37],
  popupAnchor: [0, -37],
});

var icoShell = L.icon({
  iconUrl: 'ico/shell.png',
  iconSize: [20, 20],
  iconAnchor: [10, 10],
  popupAnchor: [0, -10],

});

var icoSquare = L.icon({
  iconUrl: 'ico/square-shell.png',
  iconSize: [20, 20],
  iconAnchor: [10, 10],
  popupAnchor: [0, -10],

});

var icoHostel = L.icon({
  iconUrl: 'ico/herberge-s.png',
  iconSize: [24, 24],
  iconAnchor: [12, 12],
  popupAnchor: [0, -6],

});

var trkItems = {
  shell: icoShell,
  square: icoSquare,
  flag: icoInfo,
};

var wptItems = {
  star: icoStar,
  //flag: icoInfo,
  pilgrim: icoHostel,
};

L.Control.Position = L.Control.extend({
  onAdd: function(map) {
    var content = L.DomUtil.create('div');
    content.setAttribute('style',
      'background: rgb(1, 1, 1, 0.2);');
    content.id = 'position';
    content.textContent = 'Position';
    return content;
  },
  onRemove: function(map) {
    // nothing to do
  }
});

L.control.position = function(opts) {
  return new L.Control.Position(opts);
}

/*
 * gpxFiles: if undefined read filelist from hash
 */
function showMap(gpxFiles) {
  var mymap = L.map('mapid');
  mymap.attributionControl.setPrefix(false);
  var mapnik = L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '<a href="https://www.openstreetmap.org/">OpenStreetMap Contributors</a>',
    maxZoom: 18,
  });
  var topo = L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png', {
    attribution: '<a href="https://www.openstreetmap.org/">OpenStreetMap Contributors</a>i, ' +
    '<a href="https://www.opentopomap.org/">OpenTopoMap</a>',
    maxZoom: 18,
  });
  var hiking = L.tileLayer("https://tile.waymarkedtrails.org/hiking/{z}/{x}/{y}.png", {
    attribution: '<a href="https://www.openstreetmap.org/">OpenStreetMap Contributors</a>i, ' +
    '<a href="https://www.opentopomap.org/">OpenTopoMap</a>',
    maxZoom: 18,
  });
  mymap.addLayer(mapnik);
  var baseMaps = {
    "Mapnik": mapnik,
    "Topo": topo,
  };
  var tracks = L.layerGroup();
  var pois = L.layerGroup();
  var overlayMaps = {
    "Tracks": tracks,
    "Pois": pois,
    "Wanderwege": hiking,
  };
  mymap.addLayer(tracks);
  mymap.addLayer(pois);
  var layersControl = new L.Control.Layers(baseMaps, overlayMaps);
  mymap.addControl(layersControl);
  L.control.scale({
    imperial: false,
  }).addTo(mymap);
  L.control.position({position: 'bottomleft',}).addTo(mymap);
  var pos = getHash(mymap);
  console.log("gpx: "+pos.gpx);
  if (gpxFiles == undefined) {
    gpxFiles = pos.gpx;
  }
  mymap.setView([pos.lat, pos.lon], pos.zoom);
  mymap.on('moveend', function(e) {
    console.log("Moved to: "+mymap.getCenter());
    console.log("Zoomed to: "+mymap.getZoom());
    setHash(mymap, pos.gpx);
  });
  mymap.on('mousemove', function(e) {
    var pos = document.getElementById('position');
    pos.textContent = e.latlng.lat.toFixed(6)+", "+e.latlng.lng.toFixed(6);
  });
  for (var i = 0; i < gpxFiles.length; i++) {
    getGpx(tracks, pois, gpxFiles[i]);
  }
}

function getHash() {
  var hash = location.hash;
  console.log("hash: "+hash);
  tracks = new Array();
  if (hash.contains("map")) {
    var parts = hash.split("#map=")[1].split("/");
    console.log("parts: "+parts);
    if (parts.length > 3) {
      for (var i = 3; i < parts.length; i++) {
        tracks.push([parts[i], 'red']);
      }
    }
    return {
      zoom: parts[0],
      lat: parts[1],
      lon: parts[2],
      gpx: tracks,
    }
  } else {
    return {
      zoom: 13,
      lat: 50.8,
      lon: 12.9,
      gpx: tracks,
    }
  }
}

function setHash(map, tracks) {
  var digits=Math.ceil(2*Math.log(map.getZoom())-1);
  var hash = "#map="+map.getZoom()+"/"+map.getCenter().lat.toFixed(digits)
    +"/"+map.getCenter().lng.toFixed(digits);
  for (var i = 0; i <tracks.length; i++) {
    hash += "/"+tracks[i][0];
  }
  window.location.hash = hash;
}

function getGpx(trackLayer, poiLayer, gpxFile) {
  var xmlHttp = new XMLHttpRequest();
  var color = gpxFile[1];
  xmlHttp.open("GET", gpxFile[0], true);
  xmlHttp.overrideMimeType("application/xml");
  xmlHttp.onreadystatechange = function() {
    if (xmlHttp.readyState != 4) return;
    if (xmlHttp.status != 0 && xmlHttp.status != 200) return;
    console.log("xml received");
    var xml = xmlHttp.responseXML;
    var trk = xml.getElementsByTagName('trk');
    if (trk.length == 0) {
      trk = xml.getElementsByTagName('rte');
    }
    var name = '';
    if (trk.length > 0) {
      trk = trk[0];
      for (var el = trk.firstChild; el != null; el = el.nextSibling) {
        if (el.nodeType == Node.ELEMENT_NODE && el.nodeName == 'name') {
          name = el.firstChild.nodeValue;
          break;
        }
      }
    }
    var gpx = {
      name: name,
      trk: xml.getElementsByTagName('trkpt'),
      rte: xml.getElementsByTagName('rtept'),
      wpt: xml.getElementsByTagName('wpt'),
    }
    console.log("rte: "+gpx.rte);
    addTrack(trackLayer, gpx, color);
    addPois(poiLayer, gpx);
  };
  xmlHttp.send();
}

function addTrack(trackLayer, gpx, color) {
  var latLons = [];
  if (gpx.trk.length > 0) {
    for (var i = 0; i < gpx.trk.length; i++) {
      var lat = gpx.trk.item(i).getAttribute('lat');
      var lon = gpx.trk.item(i).getAttribute('lon');
      latLons.push([parseFloat(lat), parseFloat(lon)]);
    }
  }
  else if (gpx.rte.length > 0) {
    for (var i = 0; i < gpx.rte.length; i++) {
      var lat = gpx.rte.item(i).getAttribute('lat');
      var lon = gpx.rte.item(i).getAttribute('lon');
      latLons.push([parseFloat(lat), parseFloat(lon)]);
    }
  }
  var line = L.polyline(latLons, {
    color: color,
    opacity: 0.5,
    weight: 7,
  });
  console.log("line: "+latLons);
  trackLayer.addLayer(line);
  line.bindPopup(gpx.name);
}

function addPois(poiLayer, gpx) {
  for (var i = 0; i < gpx.trk.length; i++) {
    var poi = getPoi(gpx.trk.item(i));
    if (poi == null || trkItems[poi.sym] == undefined) continue;
    var marker = L.marker([poi.lat, poi.lon], {
      icon: trkItems[poi.sym],
    });
    marker.bindPopup(poiText(poi), {minWidth: 400});
    poiLayer.addLayer(marker);
  }
  for (var i = 0; i < gpx.rte.length; i++) {
    var poi = getPoi(gpx.rte.item(i));
    if (poi == null || trkItems[poi.sym] == undefined) continue;
    var marker = L.marker([poi.lat, poi.lon], {
      icon: trkItems[poi.sym],
    });
    marker.bindPopup(poiText(poi), {minWidth: 400});
    poiLayer.addLayer(marker);
  }
  for (var i = 0; i < gpx.wpt.length; i++) {
    var poi = getPoi(gpx.wpt.item(i));
    if (poi == null || wptItems[poi.sym] == undefined) continue;
    var marker = L.marker([poi.lat, poi.lon], {
      icon: wptItems[poi.sym],
    });
    marker.bindPopup(poiText(poi), {minWidth: 400});
    poiLayer.addLayer(marker);
  }
}

function getPoi(gpxPoint) {
  var lat = parseFloat(gpxPoint.getAttribute('lat'));
  var lon = parseFloat(gpxPoint.getAttribute('lon'));
  var sym = null;
  var name = '';
  var link = null;
  var cmt = '';
  for (var node = gpxPoint.firstChild; node != null; node = node.nextSibling) {
    if (node.nodeType != Node.ELEMENT_NODE) continue;
    if (node.nodeName == 'sym') {
      sym = node.firstChild.nodeValue;
    } else if (node.nodeName == 'name') {
      name = node.firstChild.nodeValue;
    } else if (node.nodeName == 'cmt') {
      cmt = node.firstChild.nodeValue;
    } else if (node.nodeName == 'link') {
      link = node.getAttribute('href');
    }
  }
  if (sym == null) return null;
  return {
    lon: lon,
    lat: lat,
    sym: sym,
    name: name,
    cmt: cmt,
    link: link,
  };
}

function poiText(poi) {
  var text = '<p style="font-weight: bold;">'+poi.name+'</p>';
  if (poi.link != null) {
   text += '<div><img src="'+poi.link+'" alt="'+poi.name
     +'" style=max-width:400px;"/></div>';
  }
  if (poi.cmt != '') {
    text += '<p>'+poi.cmt+'</p>';
  }
  console.log("poi: "+text);
  return text;
}
