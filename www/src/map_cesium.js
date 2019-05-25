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
}