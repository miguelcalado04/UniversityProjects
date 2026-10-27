

-------------------------------------------------------------------------------------
--PROCEDURE ADD_PATIENT
------------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE add_patient (
    p_nif BIGINT,
    p_nome VARCHAR,
    p_telemovel NUMERIC,
    p_email VARCHAR,
    p_data_nasc VARCHAR,
    p_genero VARCHAR,
    p_password VARCHAR,
    OUT p_utilizador_id INT,
    OUT p_status VARCHAR,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN
    BEGIN
	
		LOCK TABLE utilizador IN EXCLUSIVE MODE;
        LOCK TABLE utente IN EXCLUSIVE MODE;
		
        IF EXISTS (SELECT 1 FROM utilizador WHERE nif = p_nif) THEN
            p_status := 'error';
            p_message := 'NIF já existe!';
            RETURN;
        END IF;

		INSERT INTO utilizador (nif, nome, telemovel, email, password)
        VALUES (p_nif, p_nome, p_telemovel, p_email, p_password)
        RETURNING id INTO p_utilizador_id;

		IF p_utilizador_id IS NULL THEN
            RAISE EXCEPTION 'Failed to insert into utilizador table';
        END IF;

		INSERT INTO utente (data_nasc, genero, utilizador_id)
        VALUES (p_data_nasc, p_genero, p_utilizador_id);

        p_status := 'success';
        p_message := 'Paciente adicionado com sucesso!';
    EXCEPTION
        WHEN OTHERS THEN
            p_status := 'error';
            p_message := SQLERRM;
    END;
END;
$$;

-------------------------------------------------------------------------------------
--PROCEDURE ADD_ASSISTENT
------------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE add_assistant (
    p_nif BIGINT,
    p_nome VARCHAR,
    p_telemovel NUMERIC,
    p_email VARCHAR,
    p_detalhes_contrato VARCHAR,
    p_password VARCHAR,
    OUT p_utilizador_id BIGINT,
    OUT p_status VARCHAR,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN
    BEGIN
		
		LOCK TABLE utilizador IN EXCLUSIVE MODE;
        LOCK TABLE assistente IN EXCLUSIVE MODE;
		
        IF EXISTS (SELECT 1 FROM utilizador WHERE nif = p_nif) THEN
            p_status := 'error';
            p_message := 'NIF já existe!';
            RETURN;
        END IF;

        INSERT INTO utilizador (nif, nome, telemovel, email, password)
        VALUES (p_nif, p_nome, p_telemovel, p_email, p_password)
        RETURNING id INTO p_utilizador_id;

		IF p_utilizador_id IS NULL THEN
            RAISE EXCEPTION 'Failed to insert into utilizador table';
        END IF;
        -- Inserir na tabela empregado
        INSERT INTO empregado (detalhes_contrato, utilizador_id)
        VALUES (p_detalhes_contrato, p_utilizador_id);

        -- Inserir na tabela assistente
        INSERT INTO assistente (empregado_utilizador_id)
        VALUES (p_utilizador_id);

        p_status := 'success';
        p_message := 'Assistente adicionado com sucesso!';
    EXCEPTION
        WHEN OTHERS THEN
            p_status := 'error';
            p_message := SQLERRM;
    END;
END;
$$;


-------------------------------------------------------------------------------------
--PROCEDURE ADD_ENFERMEIRO
------------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE add_enfermeiro (
    p_nif BIGINT,
    p_nome VARCHAR,
    p_telemovel NUMERIC,
    p_email VARCHAR,
    p_password VARCHAR,
    p_detalhes_contrato VARCHAR,
    p_hierarquia_interna VARCHAR,
    OUT p_utilizador_id INT,
    OUT p_status VARCHAR,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN
    BEGIN
		
		LOCK TABLE utilizador IN EXCLUSIVE MODE;
        LOCK TABLE enfermeiro IN EXCLUSIVE MODE;
		
        IF EXISTS (SELECT 1 FROM utilizador WHERE nif = p_nif) THEN
            p_status := 'error';
            p_message := 'NIF já existe!';
            RETURN;
        END IF;

        INSERT INTO utilizador (nif, nome, telemovel, email, password)
        VALUES (p_nif, p_nome, p_telemovel, p_email, p_password)
        RETURNING id INTO p_utilizador_id;

		IF p_utilizador_id IS NULL THEN
            RAISE EXCEPTION 'Failed to insert into utilizador table';
        END IF;
        INSERT INTO empregado (detalhes_contrato, utilizador_id)
        VALUES (p_detalhes_contrato, p_utilizador_id);
        -- Inserir na tabela enfermeiro
        INSERT INTO enfermeiro (hierarquia_interna, empregado_utilizador_id)
        VALUES (p_hierarquia_interna, p_utilizador_id);
        p_status := 'success';
        p_message := 'Enfermeiro adicionado com sucesso!';
    EXCEPTION
        WHEN OTHERS THEN
            p_status := 'error';
            p_message := SQLERRM;
    END;
END;
$$;


-------------------------------------------------------------------------------------
--PROCEDURE ADD_MEDICO
------------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE add_medico (
    p_nif BIGINT,
    p_nome VARCHAR,
    p_telemovel NUMERIC,
    p_email VARCHAR,
    p_detalhes_contrato VARCHAR,
    p_licenca_medica VARCHAR,
    p_password VARCHAR,
    OUT p_utilizador_id INT,
    OUT p_status VARCHAR,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN
    BEGIN
		
		LOCK TABLE utilizador IN EXCLUSIVE MODE;
        LOCK TABLE medico IN EXCLUSIVE MODE;
		
        IF EXISTS (SELECT 1 FROM utilizador WHERE nif = p_nif) THEN
            p_status := 'error';
            p_message := 'NIF já existe!';
            RETURN;
        END IF;
        
        INSERT INTO utilizador (nif, nome, telemovel, email, password)
        VALUES (p_nif, p_nome, p_telemovel, p_email, p_password)
        RETURNING id INTO p_utilizador_id;

        IF p_utilizador_id IS NULL THEN
            RAISE EXCEPTION 'Failed to insert into utilizador table';
        END IF;

        -- Inserir na tabela empregado
        INSERT INTO empregado (detalhes_contrato, utilizador_id)
        VALUES (p_detalhes_contrato, p_utilizador_id);

        -- Inserir na tabela medico
        INSERT INTO medico (licenca_medica, empregado_utilizador_id)
        VALUES (p_licenca_medica, p_utilizador_id);

        p_status := 'success';
        p_message := 'Médico adicionado com sucesso!';
    EXCEPTION
        WHEN OTHERS THEN
            p_status := 'error';
            p_message := SQLERRM;
    END;
END;
$$;


-------------------------------------------------------------------------------------
--PROCEDURE GET_APPOINTEMENTS
------------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE get_patient_appointments (
    IN p_patient_user_id INT,
    OUT p_id_consulta INT,
    OUT p_id_medico INT,
    OUT p_nome_medico VARCHAR,
    OUT p_data_consulta VARCHAR,
    OUT p_status VARCHAR,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN

    IF EXISTS (SELECT 1 FROM assistente WHERE empregado_utilizador_id = p_patient_user_id) THEN
        p_status := 'error';
        p_message := 'Unauthorized access!';
        RETURN;
    END IF;

    IF EXISTS (SELECT 1 FROM utente WHERE utilizador_id = p_patient_user_id) THEN
        SELECT
            c.id_consulta,
            c.medico_empregado_utilizador_id,
            u.nome,
            c.data_hora
        INTO
            p_id_consulta,
            p_id_medico,
            p_nome_medico,
            p_data_consulta
        FROM
            consulta c
        JOIN
            utilizador u ON c.medico_empregado_utilizador_id = u.id
        WHERE
            c.utente_utilizador_id = p_patient_user_id;

        p_status := 'success';
    ELSE
        p_status := 'error';
    END IF;
END;
$$;

-------------------------------------------------------------------------------------
--PROCEDURE GET_PRESCRIPTIONS
------------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE get_prescriptions(
    IN p_person_id INT,
    OUT p_status VARCHAR,
    OUT p_results VARCHAR
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_nif_user_prescricao INT;
    v_results JSON;
BEGIN
    -- Verifica se o usuário existe
    IF NOT EXISTS (SELECT 1 FROM utilizador WHERE id = p_person_id) THEN
        p_status := 'error';
        p_results := json_build_object('message', 'User not found');
        RETURN;
    END IF;

    SELECT nif INTO v_nif_user_prescricao FROM utilizador WHERE id = p_person_id;

    SELECT json_agg(json_build_object('id', r.id_receita,'posology', (SELECT json_agg(json_build_object('medicine', md.tipo_medic,'dose', md.dosagem_dosagem ))FROM medicamento_dosagem mdWHERE md.receita_id_receita = r.id_receita)))
    INTO v_results
    FROM receita r
    WHERE r.utente_utilizador_id = p_person_id;

    p_status := 'success';
    p_results := v_results;
END;
$$;