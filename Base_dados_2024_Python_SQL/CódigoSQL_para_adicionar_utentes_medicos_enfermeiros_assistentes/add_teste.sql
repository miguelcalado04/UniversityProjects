DO $$
DECLARE
    p_utilizador_id INT;
    p_status VARCHAR;
    p_message VARCHAR;
BEGIN
   
    CALL add_patient(111444333::BIGINT, 'Paciente1'::VARCHAR, 9333::NUMERIC, 'paciente1@gmail.com'::VARCHAR, '1999-08-19'::VARCHAR, 'Masculino'::VARCHAR, 'paciente1'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(222555444::BIGINT, 'Paciente2'::VARCHAR, 9444::NUMERIC, 'paciente2@gmail.com'::VARCHAR, '1995-04-12'::VARCHAR, 'Feminino'::VARCHAR, 'paciente2'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(333666555::BIGINT, 'Paciente3'::VARCHAR, 9555::NUMERIC, 'paciente3@gmail.com'::VARCHAR, '2000-01-30'::VARCHAR, 'Masculino'::VARCHAR, 'paciente3'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(444777666::BIGINT, 'Paciente4'::VARCHAR, 9666::NUMERIC, 'paciente4@gmail.com'::VARCHAR, '1988-11-22'::VARCHAR, 'Feminino'::VARCHAR, 'paciente4'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(555888777::BIGINT, 'Paciente5'::VARCHAR, 9777::NUMERIC, 'paciente5@gmail.com'::VARCHAR, '1979-07-14'::VARCHAR, 'Masculino'::VARCHAR, 'paciente5'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(666999888::BIGINT, 'Paciente6'::VARCHAR, 9888::NUMERIC, 'paciente6@gmail.com'::VARCHAR, '1993-09-09'::VARCHAR, 'Feminino'::VARCHAR, 'paciente6'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(777000999::BIGINT, 'Paciente7'::VARCHAR, 9999::NUMERIC, 'paciente7@gmail.com'::VARCHAR, '1998-12-31'::VARCHAR, 'Masculino'::VARCHAR, 'paciente7'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(888111000::BIGINT, 'Paciente8'::VARCHAR, 9000::NUMERIC, 'paciente8@gmail.com'::VARCHAR, '1992-06-20'::VARCHAR, 'Feminino'::VARCHAR, 'paciente8'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(999222111::BIGINT, 'Paciente9'::VARCHAR, 9111::NUMERIC, 'paciente9@gmail.com'::VARCHAR, '1991-03-05'::VARCHAR, 'Masculino'::VARCHAR, 'paciente9'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_patient(333222000::BIGINT, 'Paciente10'::VARCHAR, 9222::NUMERIC, 'paciente10@gmail.com'::VARCHAR, '1985-10-10'::VARCHAR, 'Feminino'::VARCHAR, 'paciente10'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;
END;
$$;

DO $$
DECLARE
    p_utilizador_id BIGINT;
    p_status VARCHAR;
    p_message VARCHAR;
BEGIN
   
    CALL add_assistant(123456789::BIGINT, 'Assistente1'::VARCHAR, 9333::NUMERIC, 'assistente1@gmail.com'::VARCHAR, 'Detalhes Contrato 1'::VARCHAR, 'assistente1'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(987654321::BIGINT, 'Assistente2'::VARCHAR, 9444::NUMERIC, 'assistente2@gmail.com'::VARCHAR, 'Detalhes Contrato 2'::VARCHAR, 'assistente2'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(112233445::BIGINT, 'Assistente3'::VARCHAR, 9555::NUMERIC, 'assistente3@gmail.com'::VARCHAR, 'Detalhes Contrato 3'::VARCHAR, 'assistente3'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(556677889::BIGINT, 'Assistente4'::VARCHAR, 9666::NUMERIC, 'assistente4@gmail.com'::VARCHAR, 'Detalhes Contrato 4'::VARCHAR, 'assistente4'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(998877665::BIGINT, 'Assistente5'::VARCHAR, 9777::NUMERIC, 'assistente5@gmail.com'::VARCHAR, 'Detalhes Contrato 5'::VARCHAR, 'assistente5'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(554433221::BIGINT, 'Assistente6'::VARCHAR, 9888::NUMERIC, 'assistente6@gmail.com'::VARCHAR, 'Detalhes Contrato 6'::VARCHAR, 'assistente6'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(443322110::BIGINT, 'Assistente7'::VARCHAR, 9999::NUMERIC, 'assistente7@gmail.com'::VARCHAR, 'Detalhes Contrato 7'::VARCHAR, 'assistente7'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(665544332::BIGINT, 'Assistente8'::VARCHAR, 9000::NUMERIC, 'assistente8@gmail.com'::VARCHAR, 'Detalhes Contrato 8'::VARCHAR, 'assistente8'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(221133445::BIGINT, 'Assistente9'::VARCHAR, 9111::NUMERIC, 'assistente9@gmail.com'::VARCHAR, 'Detalhes Contrato 9'::VARCHAR, 'assistente9'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_assistant(334455667::BIGINT, 'Assistente10'::VARCHAR, 9222::NUMERIC, 'assistente10@gmail.com'::VARCHAR, 'Detalhes Contrato 10'::VARCHAR, 'assistente10'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;
END;
$$;

DO $$
DECLARE
    p_utilizador_id INT;
    p_status VARCHAR;
    p_message VARCHAR;
BEGIN

    CALL add_enfermeiro(110222333::BIGINT, 'Enfermeiro1'::VARCHAR, 9333::NUMERIC, 'enfermeiro1@gmail.com'::VARCHAR, 'enfermeiro1'::VARCHAR, 'Detalhes Contrato 1'::VARCHAR, 'primary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(222333444::BIGINT, 'Enfermeiro2'::VARCHAR, 9444::NUMERIC, 'enfermeiro2@gmail.com'::VARCHAR, 'enfermeiro2'::VARCHAR, 'Detalhes Contrato 2'::VARCHAR, 'secondary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(333444555::BIGINT, 'Enfermeiro3'::VARCHAR, 9555::NUMERIC, 'enfermeiro3@gmail.com'::VARCHAR, 'enfermeiro3'::VARCHAR, 'Detalhes Contrato 3'::VARCHAR, 'primary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(444555666::BIGINT, 'Enfermeiro4'::VARCHAR, 9666::NUMERIC, 'enfermeiro4@gmail.com'::VARCHAR, 'enfermeiro4'::VARCHAR, 'Detalhes Contrato 4'::VARCHAR, 'secondary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(555666777::BIGINT, 'Enfermeiro5'::VARCHAR, 9777::NUMERIC, 'enfermeiro5@gmail.com'::VARCHAR, 'enfermeiro5'::VARCHAR, 'Detalhes Contrato 5'::VARCHAR, 'primary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(666777888::BIGINT, 'Enfermeiro6'::VARCHAR, 9888::NUMERIC, 'enfermeiro6@gmail.com'::VARCHAR, 'enfermeiro6'::VARCHAR, 'Detalhes Contrato 6'::VARCHAR, 'secondary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(777888999::BIGINT, 'Enfermeiro7'::VARCHAR, 9999::NUMERIC, 'enfermeiro7@gmail.com'::VARCHAR, 'enfermeiro7'::VARCHAR, 'Detalhes Contrato 7'::VARCHAR, 'primary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(888999000::BIGINT, 'Enfermeiro8'::VARCHAR, 9000::NUMERIC, 'enfermeiro8@gmail.com'::VARCHAR, 'enfermeiro8'::VARCHAR, 'Detalhes Contrato 8'::VARCHAR, 'secondary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(999000111::BIGINT, 'Enfermeiro9'::VARCHAR, 9111::NUMERIC, 'enfermeiro9@gmail.com'::VARCHAR, 'enfermeiro9'::VARCHAR, 'Detalhes Contrato 9'::VARCHAR, 'primary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;

    CALL add_enfermeiro(000111222::BIGINT, 'Enfermeiro10'::VARCHAR, 9222::NUMERIC, 'enfermeiro10@gmail.com'::VARCHAR, 'enfermeiro10'::VARCHAR, 'Detalhes Contrato 10'::VARCHAR, 'secondary'::VARCHAR, p_utilizador_id, p_status, p_message);
    RAISE NOTICE 'Status: %, Message: %', p_status, p_message;
END;
$$;
