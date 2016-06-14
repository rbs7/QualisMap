/*
** Projeto Qualismap [UFPE - CIn]
** IF682 - Engenharia de Software e Sistemas
** Equipe: João Vicente, Rebeca Paula, Renata Caroline, Rogério Santos.
** Versão: 2.0
** Data de Criação: 14/05/2016
** Descrição: Esquema e tabelas utilizadas para armazenar as leituras do dispositivo e as análises feitas pelo servidor.
**
** Tabelas:
	Local:
		longitude, latitude, data_hora: dados do Dispositivo
		classificacao: feita pelo Servidor ou por trigger ***
** Última Alteração: 
**		Data: 11/06/2016
**		Alteração:  Remoção da tabela dispositivo, porque agora o banco só guarda o local, sua classificação e a data de recebimento da classificacao,
					não mais guardando todas as leituras dos sensores.
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