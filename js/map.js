/*
** QUALISMAP - Engenharia de Software e Sistemas
** IF682 - Engenharia de Software e Sistemas
** Equipe: João Vicente, Rebeca Paula, Renata Caroline, Rogério Santos.
** Versão: 1.0
** Descrição: arquivo do servidor responsável por gerar o mapa e marcar pontos.
** Melhorias a serem feitas: Marcar os pontos armazenados no banco de dados.
*/

// Variaveis necessarias
var map;
var infoWindow;

// A variavel markersData guarda a informacao necessaria a cada marcador
var markersData = [
   {
		lat:-8.055515, 
		lng:-34.951547,
		nome: "Centro de Informática",
		morada1:"Av. Jorn. Aníbal Fernandes, s/n",
		morada2: "Cidade Universitária,Recife - PE",
		codPostal: "50740-560"
   },
   {
		lat: -8.053497, 
		lng: -34.955152,
		nome: "Centro de Tecnologia e Geociências",
		morada1:"Av. da Arquitetura, s/n",
		morada2: "Cidade Universitária,Recife - PE",
		codPostal: "50740-550"
   },
   {
		lat: -8.056195,
		lng: -34.953135,
		nome: "Universidade Federal de Pernambuco,Av. da Arquitetura",
		morada1:"Cidade Universitária,Recife - PE",
		morada2: "Gafanha da NazarÃ©",
		codPostal: "3830-225 Gafanha da NazarÃ©"
   }
];


function initialize() {
	var mapOptions = {
		center: new google.maps.LatLng(40.601203,-8.668173),
		zoom: 9,
		mapTypeId: 'roadmap',
	};

	map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);

	// cria a nova Info Window com referencia a variavel infowindow
	// o conteudo da Info Window sera atribuido mais tarde
	infoWindow = new google.maps.InfoWindow();

	// evento que fecha a infoWindow com click no mapa
	google.maps.event.addListener(map, 'click', function() {
		infoWindow.close();
	});

   // Chamada para a funcao que vai percorrer a informacao
   // contida na variavel markersData e criar os marcadores a mostrar no mapa
   displayMarkers();
   iniciar();
}
google.maps.event.addDomListener(window, 'load', initialize);

// Esta funcao vai percorrer a informacao contida na variavel markersData
// e cria os marcadores atraves da funcao createMarker
function displayMarkers(){

   // esta variavel vai definir a area de mapa a abranger e o nivel do zoom
   // de acordo com as posicoes dos marcadores
   var bounds = new google.maps.LatLngBounds();
   
   // Loop que vai estruturar a informacao contida em markersData
   // para que a funcao createMarker possa criar os marcadores 
   for (var i = 0; i < markersData.length; i++){
	   
		var latlng = new google.maps.LatLng(markersData[i].lat, markersData[i].lng);
		var nome = markersData[i].nome;
		var morada1 = markersData[i].morada1;
		var morada2 = markersData[i].morada2;
		var codPostal = markersData[i].codPostal;

		createMarker(latlng, nome, morada1, morada2, codPostal);

		// Os valores de latitude e longitude do marcador sao adicionados a variavel bounds
		bounds.extend(latlng);  
   }

   // Depois de criados todos os marcadores
   // a API atraves da sua funcao fitBounds vai redefinir o nivel do zoom
   // e consequentemente a area do mapa abrangida.
   map.fitBounds(bounds);
}

// Funcao que cria os marcadores e define o conteudo de cada Info Window.
function createMarker(latlng, nome, morada1, morada2, codPostal){
	var marker = new google.maps.Marker({
		map: map,
		position: latlng,
		title: nome
	});

	// Evento que da instrucao a  API para estar alerta ao click no marcador.
	// Define o conteudo e abre a Info Window.
	google.maps.event.addListener(marker, 'click', function() {
	  
	// Variavel que define a estrutura do HTML a inserir na Info Window.
	var iwContent = '<div id="iw_container">' +
					'<div class="iw_title">' + nome + '</div>' +
					'<div class="iw_content">' + morada1 + '<br />' +
					 morada2 + '<br />' +
					 codPostal + '</div></div>';

	// O conteudo da variavel iwContent sera inserido na Info Window.
	infoWindow.setContent(iwContent);

	// A Info Window sera aberta.
	infoWindow.open(map, marker);
	});
}

function iniciar(){
	document.querySelector("#buscar").onclick = buscar;
}

function buscar(){
	var address = document.getElementById("address").value;
	var geocoder = new google.maps.Geocoder();
	
	geocoder.geocode( { 'address': address}, function(results, status) {
		if (status == google.maps.GeocoderStatus.OK) {
			map.setCenter(results[0].geometry.location);
			var marker = new google.maps.Marker({
				map: map,
				position: results[0].geometry.location
			});
		} else {
			alert("Houve um erro durante a busca.\nVerifique se o local foi digitado corretamente.\n" + status);
		}
	});
}