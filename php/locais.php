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
	define( 'PASSWORD', '12345' );

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

	$sql = "SELECT latitude AS lt, longitude AS lg, classificacao AS c FROM dados_local";
	$result = $PDO->query( $sql );
	$rows = $result->fetchAll();
	$qtd = COUNT($rows);
	
	if ( $result )
	{
		for ($i = 0; $i < $qtd; $i++){
			echo json_encode ($ini.$rows[$i][lt].";".$rows[$i][lg].";".$rows[$i][c].";");
		}
	}
?>