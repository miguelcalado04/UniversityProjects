CREATE TABLE faturacao (
	id_faturacao BIGSERIAL,
	PRIMARY KEY(id_faturacao)
);

CREATE TABLE utente (
	data_nasc	 VARCHAR(512) NOT NULL,
	genero	 VARCHAR(512) NOT NULL,
	utilizador_id BIGINT,
	PRIMARY KEY(utilizador_id)
);

CREATE TABLE empregado (
	detalhes_contrato VARCHAR(512) NOT NULL,
	utilizador_id	 BIGINT,
	PRIMARY KEY(utilizador_id)
);

CREATE TABLE enfermeiro (
	hierarquia_interna	 VARCHAR(512) NOT NULL,
	empregado_utilizador_id BIGINT,
	PRIMARY KEY(empregado_utilizador_id)
);

CREATE TABLE medico (
	licenca_medica		 VARCHAR(512) NOT NULL,
	empregado_utilizador_id BIGINT,
	PRIMARY KEY(empregado_utilizador_id)
);

CREATE TABLE consulta (
	id_consulta			 BIGSERIAL,
	data_hora			 TIMESTAMP NOT NULL,
	utente_utilizador_id		 BIGINT NOT NULL,
	faturacao_id_faturacao	 BIGINT NOT NULL,
	medico_empregado_utilizador_id BIGINT NOT NULL,
	PRIMARY KEY(id_consulta)
);

CREATE TABLE cirurgia (
	id_cirugia			 BIGSERIAL,
	tipo				 VARCHAR(512) NOT NULL,
	data_hora			 TIMESTAMP NOT NULL,
	num_quarto			 NUMERIC(8,2) NOT NULL,
	utente_utilizador_id		 BIGINT NOT NULL,
	faturacao_id_faturacao	 BIGINT NOT NULL,
	medico_empregado_utilizador_id BIGINT NOT NULL,
	internamento_id_internamento	 BIGINT NOT NULL,
	PRIMARY KEY(id_cirugia)
);

CREATE TABLE internamento (
	id_internamento			 BIGSERIAL,
	data_entrada			 TIMESTAMP NOT NULL,
	num_quarto			 NUMERIC(8,2) NOT NULL,
	utente_utilizador_id		 BIGINT NOT NULL,
	enfermeiro_empregado_utilizador_id BIGINT NOT NULL,
	PRIMARY KEY(id_internamento)
);

CREATE TABLE utilizador (
	id	 BIGSERIAL,
	nif	 BIGINT NOT NULL,
	nome	 VARCHAR(512) NOT NULL,
	telemovel NUMERIC(8,2) NOT NULL,
	email	 VARCHAR(512) NOT NULL,
	password	 VARCHAR(512) NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE receita (
	id_receita		 BIGSERIAL,
	data		 DATE NOT NULL,
	utente_utilizador_id BIGINT NOT NULL,
	PRIMARY KEY(id_receita)
);

CREATE TABLE medicamento_dosagem (
	id_medic		 BIGSERIAL,
	tipo_medic	 VARCHAR(512) NOT NULL,
	dosagem_id_dosagem BIGSERIAL NOT NULL,
	dosagem_dosagem	 VARCHAR(512) NOT NULL,
	receita_id_receita BIGINT NOT NULL,
	PRIMARY KEY(id_medic)
);

CREATE TABLE efeito_secundario (
	id_efeitossec NUMERIC(8,2),
	nome_efeito	 VARCHAR(512) NOT NULL,
	PRIMARY KEY(id_efeitossec)
);

CREATE TABLE especializacao_ (
	id_espcea NUMERIC(8,2),
	nome	 VARCHAR(512) NOT NULL,
	PRIMARY KEY(id_espcea)
);

CREATE TABLE assistente (
	empregado_utilizador_id BIGINT,
	PRIMARY KEY(empregado_utilizador_id)
);

CREATE TABLE pagamento (
	id_pagamento		 BIGSERIAL,
	tipo_pagamento	 VARCHAR(512) NOT NULL,
	preco			 FLOAT(8) NOT NULL,
	estado_pag		 VARCHAR(512) NOT NULL,
	data_pag		 DATE NOT NULL,
	valor_pago		 FLOAT(8) NOT NULL,
	faturacao_id_faturacao BIGINT NOT NULL,
	PRIMARY KEY(id_pagamento)
);

CREATE TABLE gravidade (
	intensidade_			 BIGINT NOT NULL,
	ocorrencias			 BOOL,
	efeito_secundario_id_efeitossec NUMERIC(8,2),
	medicamento_dosagem_id_medic	 BIGINT,
	PRIMARY KEY(efeito_secundario_id_efeitossec,medicamento_dosagem_id_medic)
);

CREATE TABLE especializacao__especializacao_ (
	especializacao__id_espcea	 NUMERIC(8,2),
	especializacao__id_espcea1 NUMERIC(8,2) NOT NULL,
	PRIMARY KEY(especializacao__id_espcea)
);

CREATE TABLE medico_especializacao_ (
	medico_empregado_utilizador_id BIGINT,
	especializacao__id_espcea	 NUMERIC(8,2),
	PRIMARY KEY(medico_empregado_utilizador_id,especializacao__id_espcea)
);

CREATE TABLE enfermeiro_consulta (
	enfermeiro_empregado_utilizador_id BIGINT,
	consulta_id_consulta		 BIGINT,
	PRIMARY KEY(enfermeiro_empregado_utilizador_id,consulta_id_consulta)
);

CREATE TABLE enfermeiro_cirurgia (
	enfermeiro_empregado_utilizador_id BIGINT,
	cirurgia_id_cirugia		 BIGINT,
	PRIMARY KEY(enfermeiro_empregado_utilizador_id,cirurgia_id_cirugia)
);

CREATE TABLE internamento_receita (
	internamento_id_internamento BIGINT NOT NULL,
	receita_id_receita		 BIGINT,
	PRIMARY KEY(receita_id_receita)
);

CREATE TABLE receita_consulta (
	receita_id_receita	 BIGINT,
	consulta_id_consulta BIGINT NOT NULL,
	PRIMARY KEY(receita_id_receita)
);

ALTER TABLE utente ADD CONSTRAINT utente_fk1 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE empregado ADD CONSTRAINT empregado_fk1 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE enfermeiro ADD CONSTRAINT enfermeiro_fk1 FOREIGN KEY (empregado_utilizador_id) REFERENCES empregado(utilizador_id);
ALTER TABLE medico ADD CONSTRAINT medico_fk1 FOREIGN KEY (empregado_utilizador_id) REFERENCES empregado(utilizador_id);
ALTER TABLE consulta ADD CONSTRAINT consulta_fk1 FOREIGN KEY (utente_utilizador_id) REFERENCES utente(utilizador_id);
ALTER TABLE consulta ADD CONSTRAINT consulta_fk2 FOREIGN KEY (faturacao_id_faturacao) REFERENCES faturacao(id_faturacao);
ALTER TABLE consulta ADD CONSTRAINT consulta_fk3 FOREIGN KEY (medico_empregado_utilizador_id) REFERENCES medico(empregado_utilizador_id);
ALTER TABLE cirurgia ADD CONSTRAINT cirurgia_fk1 FOREIGN KEY (utente_utilizador_id) REFERENCES utente(utilizador_id);
ALTER TABLE cirurgia ADD CONSTRAINT cirurgia_fk2 FOREIGN KEY (faturacao_id_faturacao) REFERENCES faturacao(id_faturacao);
ALTER TABLE cirurgia ADD CONSTRAINT cirurgia_fk3 FOREIGN KEY (medico_empregado_utilizador_id) REFERENCES medico(empregado_utilizador_id);
ALTER TABLE cirurgia ADD CONSTRAINT cirurgia_fk4 FOREIGN KEY (internamento_id_internamento) REFERENCES internamento(id_internamento);
ALTER TABLE internamento ADD CONSTRAINT internamento_fk1 FOREIGN KEY (utente_utilizador_id) REFERENCES utente(utilizador_id);
ALTER TABLE internamento ADD CONSTRAINT internamento_fk2 FOREIGN KEY (enfermeiro_empregado_utilizador_id) REFERENCES enfermeiro(empregado_utilizador_id);
ALTER TABLE utilizador ADD UNIQUE (nif, telemovel, email, password);
ALTER TABLE receita ADD CONSTRAINT receita_fk1 FOREIGN KEY (utente_utilizador_id) REFERENCES utente(utilizador_id);
ALTER TABLE medicamento_dosagem ADD UNIQUE (dosagem_id_dosagem);
ALTER TABLE medicamento_dosagem ADD CONSTRAINT medicamento_dosagem_fk1 FOREIGN KEY (receita_id_receita) REFERENCES receita(id_receita);
ALTER TABLE assistente ADD CONSTRAINT assistente_fk1 FOREIGN KEY (empregado_utilizador_id) REFERENCES empregado(utilizador_id);
ALTER TABLE pagamento ADD CONSTRAINT pagamento_fk1 FOREIGN KEY (faturacao_id_faturacao) REFERENCES faturacao(id_faturacao);
ALTER TABLE gravidade ADD CONSTRAINT gravidade_fk1 FOREIGN KEY (efeito_secundario_id_efeitossec) REFERENCES efeito_secundario(id_efeitossec);
ALTER TABLE gravidade ADD CONSTRAINT gravidade_fk2 FOREIGN KEY (medicamento_dosagem_id_medic) REFERENCES medicamento_dosagem(id_medic);
ALTER TABLE especializacao__especializacao_ ADD CONSTRAINT especializacao__especializacao__fk1 FOREIGN KEY (especializacao__id_espcea) REFERENCES especializacao_(id_espcea);
ALTER TABLE especializacao__especializacao_ ADD CONSTRAINT especializacao__especializacao__fk2 FOREIGN KEY (especializacao__id_espcea1) REFERENCES especializacao_(id_espcea);
ALTER TABLE medico_especializacao_ ADD CONSTRAINT medico_especializacao__fk1 FOREIGN KEY (medico_empregado_utilizador_id) REFERENCES medico(empregado_utilizador_id);
ALTER TABLE medico_especializacao_ ADD CONSTRAINT medico_especializacao__fk2 FOREIGN KEY (especializacao__id_espcea) REFERENCES especializacao_(id_espcea);
ALTER TABLE enfermeiro_consulta ADD CONSTRAINT enfermeiro_consulta_fk1 FOREIGN KEY (enfermeiro_empregado_utilizador_id) REFERENCES enfermeiro(empregado_utilizador_id);
ALTER TABLE enfermeiro_consulta ADD CONSTRAINT enfermeiro_consulta_fk2 FOREIGN KEY (consulta_id_consulta) REFERENCES consulta(id_consulta);
ALTER TABLE enfermeiro_cirurgia ADD CONSTRAINT enfermeiro_cirurgia_fk1 FOREIGN KEY (enfermeiro_empregado_utilizador_id) REFERENCES enfermeiro(empregado_utilizador_id);
ALTER TABLE enfermeiro_cirurgia ADD CONSTRAINT enfermeiro_cirurgia_fk2 FOREIGN KEY (cirurgia_id_cirugia) REFERENCES cirurgia(id_cirugia);
ALTER TABLE internamento_receita ADD CONSTRAINT internamento_receita_fk1 FOREIGN KEY (internamento_id_internamento) REFERENCES internamento(id_internamento);
ALTER TABLE internamento_receita ADD CONSTRAINT internamento_receita_fk2 FOREIGN KEY (receita_id_receita) REFERENCES receita(id_receita);
ALTER TABLE receita_consulta ADD CONSTRAINT receita_consulta_fk1 FOREIGN KEY (receita_id_receita) REFERENCES receita(id_receita);
ALTER TABLE receita_consulta ADD CONSTRAINT receita_consulta_fk2 FOREIGN KEY (consulta_id_consulta) REFERENCES consulta(id_consulta);

