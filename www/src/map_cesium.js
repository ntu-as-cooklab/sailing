"use strict";

function initMap()
{
    var imageryProviders = Cesium.createDefaultImageryProviderViewModels()
    Cesium.MapboxApi.defaultAccessToken = "pk.eyJ1Ijoic2VhbnN0b25lNTkyMyIsImEiOiJjaW1kOTJrZGYwMDUxdHNra2wwOGN0cTY1In0.n0m5Euk9SWbiNt4RZi8vkQ";
    Cesium.Ion.defaultAccessToken = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiI4YjhkY2QwMi02ZTY0LTQ5NmYtOGFmYy01ZjBlM2U2OTMyMjYiLCJpZCI6MTA4NzMsInNjb3BlcyI6WyJhc3IiLCJnYyJdLCJpYXQiOjE1NTc2NDkyMDR9.0fWoPtwWYd_CAPKUzyQ3TXsgnYamRGBiQTupnIOxYaQ';
    var viewer = new Cesium.Viewer('map', {
        requestRenderMode : true,
        sceneMode: Cesium.SceneMode.SCENE2D,
        imageryProviderViewModels: imageryProviders,
        selectedImageryProviderViewModel: imageryProviders[4],
    });

    // viewer.canvas.addEventListener(Cesium.ScreenSpaceEventType.RIGHT_CLICK, function(e){
    //         var mousePosition = new Cesium.Cartesian2(e.clientX, e.clientY);
    //         var ellipsoid = viewer.scene.globe.ellipsoid;
    //         var cartesian = viewer.camera.pickEllipsoid(mousePosition, ellipsoid);
    //         if (cartesian) {
    //             var cartographic = ellipsoid.cartesianToCartographic(cartesian);
    //             var longitudeString = Cesium.Math.toDegrees(cartographic.longitude).toFixed(2);
    //             var latitudeString = Cesium.Math.toDegrees(cartographic.latitude).toFixed(2);
        
    //             alert(longitudeString + ', ' + latitudeString);
    //         } else {
    //             alert('Globe was not picked');
    //         }
    //     }, false);

    // Set the initial camera view to look at Manhattan
    var initialPosition = Cesium.Cartesian3.fromDegrees(160.000, 3.000, 1.5e7);
    viewer.scene.camera.setView({
        destination: initialPosition,
        endTransform: Cesium.Matrix4.IDENTITY
    });

    viewer.screenSpaceEventHandler.setInputAction(function(movement) {
            alert("right click");
        }, Cesium.ScreenSpaceEventType.RIGHT_CLICK);

    viewer.scene.requestRender();
}