<?PHP
/*
** Projeto Qualismap [UFPE - CIn]
** IF682 - Engenharia de Software e Sistemas
** Equipe: João Vicente, Rebeca Paula, Renata Caroline, Rogério Santos.
** Versão: 1.0
** Descrição: Módulo responsável por receber post http com dados de leituras do dispositivo e inserí-los no banco de dados.
*/

//parâmetros da conexão
define( 'HOST', 'localhost' );
define( 'DB_NAME', 'qualismap' );
define( 'USER', 'root' );
define( 'PASSWORD', '12345' );

//Conexão com banco de dados
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

//obtem dado do post
$latitude = $_POST['latitude'];
$longitude= $_POST['longitude'];
$classificacao = $_POST['classificacao'];

//Verificando se o local já existe no banco de dados.
$sql = "SELECT COUNT(*) AS qtd FROM dados_local WHERE latitude = ".$latitude." AND longitude = ".$longitude;
$result = $PDO->query( $sql );
$rows = $result->fetchAll();

if ($rows[0][qtd] == 0){
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
?>