var icons = {
flag: new OpenLayers.Icon('flag.png',
  new OpenLayers.Size(32, 32),
  new OpenLayers.Pixel(-16, -16)),
star: new OpenLayers.Icon('star.png',
  new OpenLayers.Size(32, 32),
  new OpenLayers.Pixel(-16, -16)),
square: new OpenLayers.Icon('square.png',
  new OpenLayers.Size(16, 16),
  new OpenLayers.Pixel(-8, -8)),
church: new OpenLayers.Icon('church.png',
  new OpenLayers.Size(20, 20),
  new OpenLayers.Pixel(-10, -10)),
hostel: new OpenLayers.Icon('hostel.png',
  new OpenLayers.Size(32, 32),
  new OpenLayers.Pixel(-16, -16)),
shell: new OpenLayers.Icon('shell.png',
  new OpenLayers.Size(20, 20),
  new OpenLayers.Pixel(-10, -10)),
pilgrim: new OpenLayers.Icon('pilgrim-hostel.png',
  new OpenLayers.Size(32, 32),
  new OpenLayers.Pixel(-16, -16)),

};

// Function to convert normal latitude/longitude to mercator easting/northings
function getWidth() {
  if (window.innerWidth)
    return window.innerWidth;
  else if (document.documentElement && document.documentElement.clientWidth)
    return document.documentElement.clientWidth;
  else if (document.body)
    return document.body.clientWidth;
  else
    return 800;
}   

function getHeight() {
  if (window.innerHeight)
    return window.innerHeight;
  else if (document.documentElement && document.documentElement.clientHeight)
    return document.documentElement.clientHeight;
  else if (document.body)
    return document.body.clientHeight;
  else
    return 600;
}   

var map;
var lonLat;
var proj4326;
var projmerc;

function getTileURL(bounds) {
  var res = this.map.getResolution();
  var x = Math.round((bounds.left - this.maxExtent.left) / (res * this.tileSize.w));
  var y = Math.round((this.maxExtent.top - bounds.top) / (res * this.tileSize.h));
  var z = this.map.getZoom();
  var limit = Math.pow(2, z);
  if (y < 0 || y >= limit) {
    return null;
  }
  else {
    x = ((x % limit) + limit) % limit;
    var url = this.url;
    var path = z + "/" + x + "/" + y + ".png";
    if (url instanceof Array) {
      url = this.selectUrl(path, url);
    }
    return url+path;
  }
}

function loadtrack(gpxfile, lon, lat, zoom) {
  proj4326 = new OpenLayers.Projection("EPSG:4326");
  projmerc = new OpenLayers.Projection("EPSG:900913");
  lonLat = new OpenLayers.LonLat(lon, lat);
  var attrib = 'Map Data from <a href="http://www.openstreetmap.org/">'
             + 'OpenStreetMap Contributors</a> '
	     + '(<a href="http://creativecommons.org/licenses/by-sa/2.0/">'
	     + 'CC-by-SA 2.0</a>)';

  var mapDiv = document.getElementById("map");
  mapDiv.style.width = ""+getWidth()+"px";
  mapDiv.style.height = ""+getHeight()+"px";
  map = new OpenLayers.Map ("map", {
    controls:[
      new OpenLayers.Control.Navigation(),
      new OpenLayers.Control.PanZoom(),
      //new OpenLayers.Control.PanZoomBar(),
      new OpenLayers.Control.Attribution(),
      new OpenLayers.Control.LayerSwitcher(),
      new OpenLayers.Control.ArgParser(),
      new OpenLayers.Control.Permalink('Permalink'),
      new OpenLayers.Control.ScaleLine(),
      new OpenLayers.Control.MousePosition(),
      new OpenLayers.Control.Scale()
    ],
    maxExtent: new OpenLayers.Bounds(-20037508.34,-20037508.34,20037508.34,20037508.34),
    maxResolution:156543.0399,
    units:'m',
    projection: projmerc,
    displayProjection: proj4326
  } );
// Define the map layer
// Note that we use a predefined layer that will be
// kept up to date with URL changes
// Here we define just one layer, but providing a choice
// of several layers is also quite simple
// Other defined layers are OpenLayers.Layer.OSM.Mapnik and OpenLayers.Layer.OSM.Maplint
//  var layerCycle = new OpenLayers.Layer.TMS("Cycle",
//    "http://thunderflames.org/tiles/cycle/",
//    { type: 'png', getURL: getTileURL, displayOutsideMaxExtent: true});
  var layerHikeBike = new OpenLayers.Layer.TMS("Hike & Bike map",
    "http://toolserver.org/tiles/hikebike/",
    { type: 'png',
      getURL: getTileURL,
      displayOutsideMaxExtent: true,
      attribution: attrib,
    });
  var layerCycle = new OpenLayers.Layer.TMS("Cycle",
    ["http://a.tile.opencyclemap.org/cycle/",
     "http://b.tile.opencyclemap.org/cycle/",
     "http://c.tile.opencyclemap.org/cycle/"],
    { type: 'png',
      getURL: getTileURL,
      displayOutsideMaxExtent: true,
      attribution: attrib,
    });
  var layerMapnik1 = new OpenLayers.Layer.TMS("Mapnik",
    "http://tile.openstreetmap.org/",
    { numZoomLevels: 18,
      type: 'png',
      getURL: getTileURL, 
      displayOutsideMaxExtent: true,
      attribution: attrib,
    });
  var layerOSB = new OpenLayers.Layer.TMS("OpenStreetBrowser",
    "http://tiles-base.openstreetbrowser.org/tiles/basemap_base/",
    { numZoomLevels: 18,
      type: 'png',
      getURL: getTileURL, 
      displayOutsideMaxExtent: true,
      attribution: attrib,
    });
  var hillShade = new OpenLayers.Layer.TMS("HillShade",
      "http://toolserver.org/~cmarqu/hill/",
      {
       type: 'png',
       getURL: getTileURL,
       displayOutsideMaxExtent: true,
       isBaseLayer: false,
       transparent: 'true',
       visibility: 'true'});
  hillShade.setOpacity(0.8);
  map.addLayers([layerHikeBike, layerCycle, layerMapnik1, layerOSB,
    hillShade]);
  lonLat.transform(proj4326, projmerc);

  map.addControl(new OpenLayers.Control.LayerSwitcher());
  if (!map.getCenter()) {
    map.setCenter (lonLat, zoom);
  }
  OpenLayers.Request.GET({url: gpxfile, callback: showTrack, failure: errload});
}

var selectControl;
var markerlist = [];

function showTrack(req) {
  var layerStyle = OpenLayers.Util.extend({},
    OpenLayers.Feature.Vector.style['dummy']);
  layerStyle.fillOpacity = 0.2;
  layerStyle.graphicOpacity = 1;
  var styleGreen = {
    strokeColor: "#ff00ff",
    strokeWidth: 3,
    pointRadius: 6,
    pointerEvents: "visiblePainted"
  };
  var xml = OpenLayers.Format.XML.prototype.read(req.responseText);
  var gpx = xml.documentElement.getElementsByTagName("gpx");
  var trk = xml.getElementsByTagName("trk");
  var trkseg = trk[0].getElementsByTagName("trkseg");
  var trkpt = xml.getElementsByTagName("trkpt");
  var wpt = xml.getElementsByTagName("wpt");
  var vectorLayer = new OpenLayers.Layer.Vector("Track",
    {style: layerStyle, numZoomLevels:18});
  var points = [];
  for (var i = 0; i < trkpt.length; i++) {
    var lon = parseFloat(trkpt[i].getAttribute("lon"));
    var lat = parseFloat(trkpt[i].getAttribute("lat"));
    var ll = new OpenLayers.LonLat(lon, lat);
    ll.transform(proj4326, projmerc);
    points.push(new OpenLayers.Geometry.Point(ll.lon, ll.lat));
    addMarker(markerlist, trkpt[i]);
  }
  for (var i = 0; i < wpt.length; i++) {
    addMarker(markerlist, wpt[i]);
  }
  var lineFeature = new OpenLayers.Feature.Vector(
    new OpenLayers.Geometry.LineString(points), null, styleGreen);
  lineFeature.myData = "my own Data";
  map.addLayer(vectorLayer);
  vectorLayer.addFeatures([lineFeature]);
  var markers = new OpenLayers.Layer.Markers("Marker");
  map.addLayer(markers);
  for (var i = 0; i < markerlist.length; i++) {
    if (icons[markerlist[i].sym]) {
      img = icons[markerlist[i].sym].clone();
    } else {
      img = OpenLayers.Marker.defaultIcon();
    }
    var ll = new OpenLayers.LonLat(markerlist[i].lon, markerlist[i].lat);
    ll.transform(proj4326, projmerc);
    marker = new OpenLayers.Marker(
      new OpenLayers.LonLat(ll.lon, ll.lat), img);
    marker.size = new OpenLayers.Size(10, 10);
    markers.addMarker(marker);
    markerlist[i].marker = marker;
    marker.index = i;
    marker.events.register('mousedown', marker, showPopup);
  }
  selectControl = new OpenLayers.Control.SelectFeature(vectorLayer,
    {onSelect: onFeatureSelect, onUnselect: onFeatureUnselect});
}

function addMarker(markerlist, pt) {
  var lon = parseFloat(pt.getAttribute("lon"));
  var lat = parseFloat(pt.getAttribute("lat"));
  var nName = pt.getElementsByTagName("name");
  var nSym = pt.getElementsByTagName("sym");
  var nDesc = pt.getElementsByTagName("desc");
  var nLink = pt.getElementsByTagName("link");
  var name = "";
  if (nName.length > 0) {
    var sym = null;
    var desc = "";
    var link = null;
    var name = nName[0].firstChild.nodeValue;
    if (nSym.length > 0 && nSym[0].firstChild != null) {
      sym = nSym[0].firstChild.nodeValue;
    }
    if (nDesc.length > 0 && nDesc[0].firstChild != null) {
      desc = nDesc[0].firstChild.nodeValue;
    }
    if (nLink.length > 0 && nLink[0].firstChild != null) {
      link = nLink[0].firstChild.nodeValue;
    }
    markerlist.push({lat: lat, lon: lon, name: name, sym: sym,
      desc: desc, link: link});
  }
}

var popup = null;

function showPopup(evt) {
  if (!this) return;
  if (popup)
    popup.destroy();
  var i = this.index;
  var height = 150;
  var html = "<h2>"+markerlist[i].name+"</h2>\n"+
	 "<div>"+markerlist[i].desc+"</div>\n";
  if (markerlist[i].link != null) {
    html += "<div class=\"popup\"><img src=\""+markerlist[i].link+"\"/></div>"
    height = 350;
  }
  popup = new OpenLayers.Popup.FramedCloud("blub"+i,
    this.lonlat, new OpenLayers.Size(250, height),
    html, this, true, popupClose);
  map.addPopup(popup);
}

function popupClose() {
  popup.destroy();
  popup = null;
}

var selectedFeature;

function onFeatureSelect(feature) {
  selectedFeature = feature;
  popup = new OpenLayers.Popup.AnchoredBubble("chicken",
    feature.geometry.getBounds().getCenterLonLat(),
    new OpenLayers.Size(250, 75),
    "<div>"+feature.myData+"</div>",
    null, true, onPopupClose);
  feature.popup = popup;
  map.addPopup(popup);
}

function onFeatureUnselect(feature) {
  map.removePopup(feature.popup);
  feature.popup.destroy();
  feature.popup = null;
}

function onPopupClose(evt) {
  selectControl.unselect(selectedFeature);
}

function errload(req) {
  alert(req.responseText);
}

onresize = function(evt) {
  var mapDiv = document.getElementById("map");
  mapDiv.style.width = ""+getWidth()+"px";
  mapDiv.style.height = ""+getHeight()+"px";
}

