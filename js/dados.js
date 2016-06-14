/*
** QUALISMAP - Engenharia de Software e Sistemas
** IF682 - Engenharia de Software e Sistemas
** Equipe: João Vicente, Rebeca Paula, Renata Caroline, Rogério Santos.
** Versão: 1.0
** Descrição: arquivo do servidor responsável por receber os dados do banco de dados e gerar os pontos com suas classificações.
** Melhorias a serem feitas: Gerar os pontos, porque só recebe os dados.
*/

function reqListener () {
	console.log(this.responseText);
}

var oReq = new XMLHttpRequest();

oReq.onload = function() {	
	var r = this.responseText;
	
	var lat;
	var lng;
	var clf;
		
	for (var j = 0; j < r.length; j++ ){
		lat = "";
		lng = "";
		clf = "";
		for(var i = j; r[i] != ';'; i++){
			if  (r[i] != '"')
			{
				lat += r[i];
			}
		}
		
		j = i+1;
		for(var i = j; r[i] != ';'; i++){
			if  (r[i] != '"')
			{
				lng += r[i];
			}
		}
		
		j = i+1;
		for(var i = j; r[i] != ';'; i++){
			if  (r[i] != '"')
			{
				clf += r[i];
			}
		}
		j = i;
		
		alert("Latitude : "+lat+" Longitude: "+lng+" Classificacao: "+clf);
	}
};

oReq.open("get", "php/locais.php", true);

oReq.send();