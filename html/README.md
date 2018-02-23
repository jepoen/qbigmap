# Template for displaying online maps

Install leaflet into subdirectory leaflet

Use URL

map-template.html#map=zoom/lat/lon/track1[/track2...]

(all in red), or add tracks to map-template.html:

```html
<body onload="showMap([
  [gpxFile, color],
  ...
]);">
```
