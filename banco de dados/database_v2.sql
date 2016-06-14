/*
** Projeto Qualismap [UFPE - CIn]
** IF682 - Engenharia de Software e Sistemas
** Equipe: Jo�o Vicente, Rebeca Paula, Renata Caroline, Rog�rio Santos.
** Vers�o: 2.0
** Data de Cria��o: 14/05/2016
** Descri��o: Esquema e tabelas utilizadas para armazenar as leituras do dispositivo e as an�lises feitas pelo servidor.
**
** Tabelas:
	Local:
		longitude, latitude, data_hora: dados do Dispositivo
		classificacao: feita pelo Servidor ou por trigger ***
** �ltima Altera��o: 
**		Data: 11/06/2016
**		Altera��o:  Remo��o da tabela dispositivo, porque agora o banco s� guarda o local, sua classifica��o e a data de recebimento da classificacao,
					n�o mais guardando todas as leituras dos sensores.
*/

DROP SCHEMA `qualismap`;

CREATE SCHEMA `qualismap` ;

CREATE TABLE `qualismap`.`dados_local`(
	latitude DOUBLE PRECISION(8,6),
	longitude DOUBLE PRECISION(8,6),
	data_hora TIMESTAMP,
	classificacao VARCHAR(2) NOT NULL,
	CHECK (classificacao = 'RE' OR classificacao = 'PR' OR classificacao = 'NR'),
	PRIMARY KEY (latitude, longitude)
);