
-------------------------------------------------------------------------------------
--TRIGGER GERAR FATURAÇÃO QUANDO INSERIMOS CONSULTA/CIRURGIA
------------------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION gerar_id_faturacao_cirurgia()
RETURNS TRIGGER AS $$
DECLARE
    preco_procedimento INTEGER;
    hora_ja_usada BOOLEAN;
    enfermeiro_ocupado BOOLEAN;
BEGIN

	LOCK TABLE cirurgia IN EXCLUSIVE MODE;
    LOCK TABLE consulta IN SHARE MODE;
    LOCK TABLE internamento IN SHARE MODE;
    LOCK TABLE enfermeiro_consulta IN SHARE MODE;
    LOCK TABLE enfermeiro_cirurgia IN SHARE MODE;

    SELECT EXISTS (
        SELECT 1 
        FROM cirurgia 
        WHERE medico_empregado_utilizador_id = NEW.medico_empregado_utilizador_id
          AND data_hora = NEW.data_hora
    ) INTO hora_ja_usada;

    IF hora_ja_usada THEN
        RAISE EXCEPTION 'Médico já tem cirurgia na hora pretendida';
    END IF;

    SELECT EXISTS (
        SELECT 1 
        FROM consulta 
        WHERE medico_empregado_utilizador_id = NEW.medico_empregado_utilizador_id
          AND data_hora = NEW.data_hora
    ) INTO hora_ja_usada;

    IF hora_ja_usada THEN
        RAISE EXCEPTION 'Médico já tem consulta na hora pretendida';
    END IF;


	SELECT EXISTS (
        SELECT 1 
        FROM enfermeiro_consulta ec
        JOIN consulta c ON ec.consulta_id_consulta = c.id_consulta
        WHERE ec.enfermeiro_empregado_utilizador_id IN (
            SELECT enfermeiro_empregado_utilizador_id 
            FROM enfermeiro_cirurgia 
            WHERE cirurgia_id_cirugia = NEW.id_cirugia
        )
        AND c.data_hora = NEW.data_hora
    ) INTO enfermeiro_ocupado;

    IF enfermeiro_ocupado THEN
        RAISE EXCEPTION 'Enfermeiro já está envolvido em outra consulta na hora pretendida';
    END IF;

    SELECT EXISTS (
        SELECT 1 
        FROM enfermeiro_cirurgia ec
        JOIN cirurgia c ON ec.cirurgia_id_cirugia = c.id_cirugia
        WHERE ec.enfermeiro_empregado_utilizador_id IN (
            SELECT enfermeiro_empregado_utilizador_id 
            FROM enfermeiro_cirurgia 
            WHERE cirurgia_id_cirugia = NEW.id_cirugia
        )
        AND c.data_hora = NEW.data_hora
    ) INTO enfermeiro_ocupado;

    IF enfermeiro_ocupado THEN
        RAISE EXCEPTION 'Enfermeiro já está envolvido em outra cirurgia na hora pretendida';
    END IF;


	SELECT EXISTS (
        SELECT 1 
        FROM internamento i
        WHERE i.enfermeiro_empregado_utilizador_id IN (
            SELECT enfermeiro_empregado_utilizador_id 
            FROM enfermeiro_cirurgia 
            WHERE cirurgia_id_cirugia = NEW.id_cirugia
        )
        AND i.data_entrada <= NEW.data_hora
    ) INTO enfermeiro_ocupado;

    IF enfermeiro_ocupado THEN
        RAISE EXCEPTION 'Enfermeiro já está envolvido em um internamento na hora pretendida';
    END IF;

    INSERT INTO faturacao DEFAULT VALUES RETURNING id_faturacao INTO NEW.faturacao_id_faturacao;

    preco_procedimento := 500; -- Preço para cirurgia

    INSERT INTO pagamento (tipo_pagamento, preco, estado_pag, data_pag, faturacao_id_faturacao)
    VALUES ('Cirurgia', preco_procedimento, 'Pendente', CURRENT_DATE, NEW.faturacao_id_faturacao);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER trigger_gerar_id_faturacao_cirurgia12
BEFORE INSERT ON cirurgia
FOR EACH ROW
EXECUTE FUNCTION gerar_id_faturacao_cirurgia();



------------------ CONSULTA ---------------------------

CREATE OR REPLACE FUNCTION gerar_id_faturacao_consulta()
RETURNS TRIGGER AS $$
DECLARE
    preco_procedimento INTEGER;
    hora_ja_usada BOOLEAN;
    enfermeiro_ocupado BOOLEAN;
BEGIN
    LOCK TABLE consulta IN EXCLUSIVE MODE;
    LOCK TABLE cirurgia IN EXCLUSIVE MODE;
    LOCK TABLE enfermeiro_consulta IN EXCLUSIVE MODE; -- Adicionando bloqueio na tabela enfermeiro_consulta

    -- Verifica se o médico já tem uma consulta agendada no mesmo horário
    SELECT EXISTS (
        SELECT 1 
        FROM consulta 
        WHERE medico_empregado_utilizador_id = NEW.medico_empregado_utilizador_id
          AND data_hora = NEW.data_hora
    ) INTO hora_ja_usada;

    IF hora_ja_usada THEN
        RAISE EXCEPTION 'Médico já tem consulta na hora pretendida';
    END IF;

    -- Verifica se o médico já tem uma cirurgia agendada no mesmo horário
    SELECT EXISTS (
        SELECT 1 
        FROM cirurgia 
        WHERE medico_empregado_utilizador_id = NEW.medico_empregado_utilizador_id
          AND data_hora = NEW.data_hora
    ) INTO hora_ja_usada;

    IF hora_ja_usada THEN
        RAISE EXCEPTION 'Médico já tem cirurgia na hora pretendida';
    END IF;

    -- Verifica se o enfermeiro já está envolvido em outra consulta no mesmo horário
    SELECT EXISTS (
        SELECT 1 
        FROM enfermeiro_consulta 
        WHERE enfermeiro_empregado_utilizador_id IN (
            SELECT enfermeiro_empregado_utilizador_id 
            FROM enfermeiro_consulta 
            WHERE consulta_id_consulta = NEW.id_consulta
        )
        AND consulta_id_consulta IN (
            SELECT id_consulta 
            FROM consulta 
            WHERE data_hora = NEW.data_hora
        )
    ) INTO enfermeiro_ocupado;

    IF enfermeiro_ocupado THEN
        RAISE EXCEPTION 'Enfermeiro já está envolvido em outra consulta na hora pretendida';
    END IF;

    -- Gerar ID de faturação
    INSERT INTO faturacao DEFAULT VALUES RETURNING id_faturacao INTO NEW.faturacao_id_faturacao;

    -- Definiro preço do procedimento para a consulta
    preco_procedimento := 100; -- Preço para consulta

    -- Inserir no registo de pagamento
    INSERT INTO pagamento (tipo_pagamento, preco, estado_pag, data_pag, faturacao_id_faturacao)
    VALUES ('Consulta', preco_procedimento, 'Pendente', CURRENT_DATE, NEW.faturacao_id_faturacao);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;



CREATE TRIGGER trigger_gerar_id_faturacao_consulta18
BEFORE INSERT ON consulta
FOR EACH ROW
EXECUTE FUNCTION gerar_id_faturacao_consulta();

