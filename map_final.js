// Esse arquivo é responsável por fazer a chamada à API do Google Maps e cuidar das interações da página 

var geocoder;
var infoWindow;
var map;
var marker;
var markersData = [{lat:null, lng:null, clf: null}]; // guarda a informacao necessária a cada marcador

//Recebendo dados do banco de dados
function reqListener () {
	console.log(this.responseText);
}

var oReq = new XMLHttpRequest();

oReq.onload = function() {	
	var r = this.responseText; //"0.01;0;RE;";
	r = r.substring(1, r.length-1);
	
	var latitude;
	var longitude;
	var classificacao;
	var i, j, cont = 0;
	
	for (j = 0; j < r.length; j++ )
	{
		latitude = "";
		longitude = "";
		classificacao = "";
		
		for(i = j; r[i] != ';'; i++)
		{
			if  (r[i] != '"')
			{
				latitude += r[i];
			}
		}
		
		j = i+1;
		for(i = j; r[i] != ';'; i++)
		{
			if  (r[i] != '"')
			{
				longitude += r[i];
			}
		}
		
		j = i+1;
		for(i = j; r[i] != ';'; i++)
		{
			if  (r[i] != '"')
			{
				classificacao += r[i];
			}
		}
		j = i;
		markersData[markersData.length] = {lat: parseFloat(latitude), lng: parseFloat(longitude), clf: classificacao};
	}
	
	initialize()
};

oReq.open("get", "php/locais.php", true);

oReq.send();

//Construção dos marcadores

// Esta funcao vai percorrer a informacao contida na variavel markersData e cria os marcadores atraves da funcao createMarker
function displayMarkers(){	
   // esta variavel vai definir a area de mapa a abranger e o nivel do zoom de acordo com as posicoes dos marcadores
   var bounds = new google.maps.LatLngBounds();
   // Loop que vai estruturar a informacao contida em markersData para que a funcao createMarker possa criar os marcadores 
   for (var i = 1; i < markersData.length; i++){
		//separar informacao do vetor de pontos
		var latlng = new google.maps.LatLng(markersData[i].lat, markersData[i].lng);
		var classificacao = markersData[i].clf;

		//cria o marcador
		createMarker(latlng, classificacao);

		// Os valores de latitude e longitude do marcador sao adicionados a variavel bounds
		bounds.extend(latlng);
   }
   // Depois de criados todos os marcadores a API atraves da sua funcao fitBounds vai redefinir o nivel do zoom e consequentemente a area do mapa abrangida.
   map.fitBounds(bounds);
}

// Funcao que cria os marcadores e define o conteudo de cada Info Window.
function createMarker(latlng, clf){
	var imagem;
	var classificacao;
	
	if (clf == "RE"){
		imagem = 'img/medio.png';
		classificacao = "MEDIO";
	}
	else if(clf == "PR")
	{
		imagem = 'img/ruim.png';
		classificacao = "RUIM";
	}
	else
	{
		imagem = 'img/pessimo.png';
		classificacao = "PÉSSIMO";
	}
	
	var marker = new google.maps.Marker({
		map: map,
		position: latlng,
		title: classificacao,
		icon: imagem
	});
}
	
/* Função de inicialização do mapa */
function initialize() {
	var latlng = new google.maps.LatLng(-8.055484, -34.951451);  //coordenadas do CIn
	var options = {
		zoom: 17,
		center: latlng,
		mapTypeId: google.maps.MapTypeId.ROADMAP
	};
	
	map = new google.maps.Map(document.getElementById('map-canvas'), options);
	
	geocoder = new google.maps.Geocoder();
	
	marker = new google.maps.Marker({
		map: map,
		draggable: true,
	});
	
	marker.setPosition(latlng);
	
	displayMarkers();
}

$(document).ready(function () {

	initialize();
	
	/* Função responsável por pegar o endereço, passá-lo para a API utilizando o geocoder.geocode
	que por sua vez retorna a latitude e longitude do endereço digitado*/
	function carregarNoMapa(endereco) {
		geocoder.geocode({ 'address': endereco + ', Brasil', 'region': 'BR' }, function (results, status) {
			if (status == google.maps.GeocoderStatus.OK) {
				if (results[0]) {
					 latitude = results[0].geometry.location.lat();
					 longitude = results[0].geometry.location.lng();
		
					$('#txtEndereco').val(results[0].formatted_address);
					$('#txtLatitude').val(latitude);
                   	$('#txtLongitude').val(longitude);
		
					var location = new google.maps.LatLng(latitude, longitude);
					marker.setPosition(location);
					map.setCenter(location);
					map.setZoom(16);
				}
			}
		})
	}
	
	$("#btnEndereco").click(function() {
		if($(this).val() != "")
			carregarNoMapa($("#txtEndereco").val());
	})
	
	$("#txtEndereco").blur(function() {
		if($(this).val() != "")
			carregarNoMapa($(this).val());
	})
	
	google.maps.event.addListener(marker, 'drag', function () {
		geocoder.geocode({ 'latLng': marker.getPosition() }, function (results, status) {
			if (status == google.maps.GeocoderStatus.OK) {
				if (results[0]) {  
					$('#txtEndereco').val(results[0].formatted_address);
					$('#txtLatitude').val(marker.getPosition().lat());
					$('#txtLongitude').val(marker.getPosition().lng());
				}
			}
		});
	});
	
	/* autocomplete ajuda o usuário, mostrando algumas sugestões de endereços baseadas no endereço digitado
	pelo usuário*/
	$("#txtEndereco").autocomplete({
		source: function (request, response) {
			geocoder.geocode({ 'address': request.term + ', Brasil', 'region': 'BR' }, function (results, status) {
				response($.map(results, function (item) {
					return {
						label: item.formatted_address,
						value: item.formatted_address,
						latitude: item.geometry.location.lat(),
          				longitude: item.geometry.location.lng()
					}
				}));
			})
		},
		select: function (event, ui) {
			$("#txtLatitude").val(ui.item.latitude);
    		$("#txtLongitude").val(ui.item.longitude);
			var location = new google.maps.LatLng(ui.item.latitude, ui.item.longitude);
			marker.setPosition(location);
			map.setCenter(location);
			map.setZoom(16);
		}
	});
	
	$("form").submit(function(event) {
		event.preventDefault();
		
		var endereco = $("#txtEndereco").val();
		var latitude = $("#txtLatitude").val();
		var longitude = $("#txtLongitude").val();
		
		alert("Endereço: " + endereco + "\nLatitude: " + latitude + "\nLongitude: " + longitude);
	});

});



