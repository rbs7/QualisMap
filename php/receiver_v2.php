<?php
/*
** QUALISMAP - Engenharia de Software e Sistemas
** IF682 - Engenharia de Software e Sistemas
** Equipe: João Vicente, Rebeca Paula, Renata Caroline, Rogério Santos.
** Versão: 1.0
** Descrição: Módulo responsável por encontrar todos os locais marcados para que sejam mostrados no mapa e enviar para o arquivo dados.js.
*/

	//parametros da conexao
	define( 'HOST', 'localhost' );
	define( 'DB_NAME', 'qualismap' );
	define( 'USER', 'root' );
	define( 'PASSWORD', '' );

	//Conexao com banco de dados
	try{
		$PDO = new PDO( 'mysql:host=' . HOST . ';dbname=' . DB_NAME, USER, PASSWORD );
		$PDO->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
		
		$stmt = $PDO->prepare("use qualismap"); 
		$result = $stmt->execute();	
		if ( ! $result )
		{
			print('<p>DataBase ERROR: Qualismap não está disponível...<p>');
			var_dump( $stmt->errorInfo() );
			exit;
		}
	}
	catch ( PDOException $e )
	{
		echo '<p>Erro ao conectar com o Banco de Dados.<p>' . $e->getMessage();
	}

	$latitude = $_GET["latitude"];
	$longitude = $_GET["longitude"];
	$data = $_GET["data"];
	$hora = $_GET["hora"];
	$classificacao = $_GET["classificacao"];


	//Verificando se o local já existe no banco de dados.
	$sql = "SELECT COUNT(*) AS qtd FROM dados_local WHERE latitude = ".$latitude." AND longitude = ".$longitude;
	$result = $PDO->query( $sql );
	$rows = $result->fetchAll();

	if ($rows[0][0] == 0){
		//Inserir novo local
		$sql = "INSERT INTO dados_local(latitude, longitude, data_hora, classificacao) VALUES (".$latitude.", ".$longitude.", SYSDATE(), '".$classificacao."')";
		$stmt = $PDO->prepare($sql); 
		$result = $stmt->execute();	
		if ( !$result )
		{
			var_dump( $stmt->errorInfo() );
			exit;
		}
	}
	else{
		//Atualizar local existente
		$sql = "UPDATE dados_local SET data_hora = SYSDATE() WHERE latitude = ".$latitude." AND longitude = ".$longitude."; UPDATE dados_local SET classificacao = '".$classificacao."' WHERE latitude = ".$latitude." AND longitude = ".$longitude;
		$stmt = $PDO->prepare($sql); 
		$result = $stmt->execute();	
		if ( !$result )
		{
			var_dump( $stmt->errorInfo() );
			exit;
		}
	}



	$PDO = null;
?>