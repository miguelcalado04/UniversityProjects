from flask_jwt_extended import create_access_token, jwt_required, get_jwt_identity
from flask_jwt_extended import JWTManager
from datetime import timedelta
from datetime import datetime
import flask
import logging

import psycopg2
import random
import time
import bcrypt

app = flask.Flask(__name__)
app.config['JWT_SECRET_KEY'] = 'super-secret-key'
app.config['JWT_ACCESS_TOKEN_EXPIRES'] = timedelta(hours=1)
jwt = JWTManager(app)

StatusCodes = {
    'success': 200,
    'api_error': 400,
    'internal_error': 500
}

##########################################################
## DATABASE ACCESS
##########################################################

logging.basicConfig(filename='log_file.log')
logger = logging.getLogger('logger')
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s [%(levelname)s]:  %(message)s', '%H:%M:%S')
ch.setFormatter(formatter)
logger.addHandler(ch)

def db_connection():
    db = psycopg2.connect(
        user='projeto',
        password='projeto',
        host='127.0.0.1',
        port='5432',
        database='projeto'
    )

    return db

##########################################################
## ENDPOINTS
##########################################################



def authenticate(nif, password):
    conn = None
    try:
        conn = db_connection()  # Assumindo que essa função estabelece a conexão com o banco de dados
        cur = conn.cursor()

        statement = 'SELECT password FROM utilizador WHERE nif = %s'
        cur.execute(statement, (nif,))
        result = cur.fetchone()

        if result:
            # Recupera a senha armazenada
            stored_password = result[0]
            print("Stored Password:", stored_password)

            # Verifica se a senha fornecida corresponde à senha criptografada armazenada
            if bcrypt.checkpw(password.encode('utf-8'), stored_password.encode('utf-8')):
                return True
            else:
                return False
        else:
            return False
    except (Exception, psycopg2.DatabaseError) as error:
        print('Authentication error:', error)
        return False
    finally:
        if conn is not None:
            conn.close()

##################################################################
## Endpoint para realizar login e gerar um token de acesso JWT
##################################################################

@app.route('/user', methods=['PUT'])
def login():
    logger.info('PUT /login')
    data = flask.request.get_json()
    nif = data.get('nif')
    password = data.get('password')

    if authenticate(nif, password):
        logger.info(f'User {nif} authenticated successfully')
        access_token = create_access_token(identity=nif)
        return flask.jsonify({'access_token': access_token}), 200
    else:
        logger.warning(f'Failed login attempt for NIF {nif}')
        return flask.jsonify({'message': 'Invalid credentials'}), 401

#/////////////////////////// VERFICAR UTILIZADORES ///////////////////////////////////////

def is_patient(nif):

    conn = db_connection()
    cur = conn.cursor()

    try:
        statement = '''SELECT u.id 
        FROM utilizador u
        JOIN utente p ON u.id = p.utilizador_id
        WHERE u.nif = %s
        '''
        cur.execute(statement, (nif,))
        user_id = cur.fetchone()

        if user_id:
            return True
        else:
            return False

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'Check patient error: {error}')
        return False

    finally:
        if conn is not None:
            conn.close()

def is_assistant(nif):

    conn = db_connection()
    cur = conn.cursor()

    try:
        statement = '''SELECT u.id 
                       FROM utilizador u
                       JOIN assistente a ON u.id = a.empregado_utilizador_id
                       WHERE u.nif = %s'''
        cur.execute(statement, (nif,))
        user_id = cur.fetchone()

        return user_id is not None

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'Check assistant error: {error}')
        return False

    finally:
        if conn is not None:
            conn.close()


def is_medico(nif):

    conn = db_connection()
    cur = conn.cursor()

    try:
        statement = '''SELECT u.id 
                       FROM utilizador u
                       JOIN medico m ON u.id = m.empregado_utilizador_id
                       WHERE u.nif = %s'''
        cur.execute(statement, (nif,))
        user_id = cur.fetchone()

        return user_id is not None

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'Check doctor error: {error}')
        return False

    finally:
        if conn is not None:
            conn.close()


#//////////////////////////////////////////////////////////////////////////////////




##################################################################
## Endpoint para adicionar um paciente à base de dados
##################################################################
@app.route('/patient', methods=['POST'])
def add_patient():
    logger.info('POST /patient')
    payload = flask.request.get_json()

    required_fields = ['nif', 'nome', 'telemovel', 'email', 'data_nasc', 'genero', 'password']
    missing_fields = [field for field in required_fields if field not in payload]

    if missing_fields:
        response = {'status': 'error', 'errors': f'Missing fields: {", ".join(missing_fields)}'}
        return flask.jsonify(response), 400

    conn = db_connection()
    cur = conn.cursor()

    logger.debug(f'POST /patient - payload: {payload}')

    try:

        salt = bcrypt.gensalt()
        encrypted_password = bcrypt.hashpw(payload['password'].encode('utf-8'), salt)
        encrypted_password = encrypted_password.decode('utf-8')

        # Call the stored procedure
        cur.execute("""
            CALL add_patient(%s::BIGINT, %s, %s::NUMERIC, %s, %s, %s, %s::VARCHAR, %s, %s, %s)
        """, [
            payload['nif'],
            payload['nome'],
            payload['telemovel'],
            payload['email'],
            payload['data_nasc'],
            payload['genero'],
            encrypted_password,
            None,
            None,
            None
        ])


        result = cur.fetchone()
        response = {
            'status': result[1],
            'results': result[2]
        }

        if result[1] == 'success':
            response['utilizador_id'] = result[0]

        conn.commit()

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /add_patient - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()

    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


##################################################################
## Endpoint para adicionar um assistente à base de dados
##################################################################
@app.route('/assistant', methods=['POST'])
def add_assistant():
    logger.info('POST /assistant')
    payload = flask.request.get_json()

    required_fields = ['nif', 'nome', 'telemovel', 'email', 'detalhes_contrato', 'password']
    missing_fields = [field for field in required_fields if field not in payload]

    if missing_fields:
        response = {'status': 'error', 'errors': f'Missing fields: {", ".join(missing_fields)}'}
        return flask.jsonify(response), 400

    conn = db_connection()
    cur = conn.cursor()

    logger.debug(f'POST /assistant - payload: {payload}')

    try:

        salt = bcrypt.gensalt()
        encrypted_password = bcrypt.hashpw(payload['password'].encode('utf-8'), salt)
        encrypted_password = encrypted_password.decode('utf-8')

        cur.execute("""
            CALL add_assistant(%s, %s, %s, %s, %s, %s::VARCHAR, %s, %s, %s)
        """, [
            payload['nif'],
            payload['nome'],
            payload['telemovel'],
            payload['email'],
            payload['detalhes_contrato'],
            encrypted_password,
            None,
            None,
            None
        ])


        result = cur.fetchone()
        response = {
            'status': result[1],
            'results': result[2]
        }

        if result[1] == 'success':
            response['utilizador_id'] = result[0]
        conn.commit()

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /assistant - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()

    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)



##################################################################
## Endpoint para adicionar um enfermeiro à base de dados
##################################################################
@app.route('/nurse', methods=['POST'])
def add_nurse():
    logger.info('POST /nurse')
    payload = flask.request.get_json()

    required_fields = ['nif', 'nome', 'telemovel', 'email', 'password', 'detalhes_contrato', 'hierarquia_interna']
    missing_fields = [field for field in required_fields if field not in payload]

    if missing_fields:
        response = {'status': 'error', 'errors': f'Missing fields: {", ".join(missing_fields)}'}
        return flask.jsonify(response), 400

    conn = db_connection()
    cur = conn.cursor()

    logger.debug(f'POST /nurse - payload: {payload}')

    try:
        salt = bcrypt.gensalt()
        encrypted_password = bcrypt.hashpw(payload['password'].encode('utf-8'), salt)
        encrypted_password = encrypted_password.decode('utf-8')


        cur.execute("""
            CALL add_enfermeiro(%s, %s, %s, %s, %s::VARCHAR, %s, %s, %s, %s, %s)
        """, [
            payload['nif'],
            payload['nome'],
            payload['telemovel'],
            payload['email'],
            encrypted_password,
            payload['detalhes_contrato'],
            payload['hierarquia_interna'],
            None,
            None,
            None
        ])


        result = cur.fetchone()
        response = {
            'status': result[1],
            'results': result[2]
        }

        if result[1] == 'success':
            response['utilizador_id'] = result[0]

        conn.commit()

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /nurse - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()

    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


##################################################################
## Endpoint para adicionar um médico à base de dados
##################################################################
@app.route('/doctor', methods=['POST'])
def add_medico():
    logger.info('POST /doctor')
    payload = flask.request.get_json()

    required_fields = ['nif', 'nome', 'telemovel', 'email', 'detalhes_contrato', 'licenca_medica', 'password']
    missing_fields = [field for field in required_fields if field not in payload]

    if missing_fields:
        response = {'status': 'error', 'errors': f'Missing fields: {", ".join(missing_fields)}'}
        return flask.jsonify(response), 400

    conn = db_connection()
    cur = conn.cursor()

    logger.debug(f'POST /doctor - payload: {payload}')

    try:
        salt = bcrypt.gensalt()
        encrypted_password = bcrypt.hashpw(payload['password'].encode('utf-8'), salt)
        encrypted_password = encrypted_password.decode('utf-8')

        # Call the stored procedure
        cur.execute("""
            CALL add_medico(%s, %s, %s, %s, %s, %s, %s::VARCHAR, %s, %s, %s)
        """, [
            payload['nif'],
            payload['nome'],
            payload['telemovel'],
            payload['email'],
            payload['detalhes_contrato'],
            payload['licenca_medica'],
            encrypted_password,
            None,
            None,
            None
        ])

        result = cur.fetchone()
        response = {
            'status': result[1],
            'results': result[2]
        }

        if result[1] == 'success':
            response['utilizador_id'] = result[0]

        conn.commit()

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /doctor - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()

    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)

@app.route('/appointment', methods=['POST'])
@jwt_required()
def create_appointment():
    logger.info('POST /appointment')

    patient_nif = get_jwt_identity()  # Obter o NIF do usuário autenticado
    conn = db_connection()
    cur = conn.cursor()

    try:
        # Obter ID do paciente pelo NIF
        statement_patient_id = 'SELECT id FROM utilizador WHERE nif = %s'
        cur.execute(statement_patient_id, (patient_nif,))
        d_id = cur.fetchone()

        if not d_id:
            response = {'status': 'error', 'message': 'Paciente não encontrado'}
            return flask.jsonify(response), StatusCodes['api_error']

        d_id = d_id[0]

        # Verificar se o usuário é um paciente
        if not is_patient(patient_nif):
            response = {'status': 'error', 'message': 'Apenas pacientes podem criar consultas'}
            return flask.jsonify(response), 403

        # Extrair dados do payload da requisição
        data = flask.request.get_json()
        doctor_id = data.get('doctor_id')
        appointment_date = data.get('date')
        nurse_ids = data.get('nurse_ids')  # Lista de IDs de enfermeiros

        # Verificar se todos os campos obrigatórios estão presentes
        if not all([doctor_id, appointment_date]):
            response = {'status': 'error', 'errors': 'Campos obrigatórios ausentes'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Definir o nível de isolamento da sessão
        conn.set_isolation_level(psycopg2.extensions.ISOLATION_LEVEL_SERIALIZABLE)

        # Iniciar uma transação
        cur.execute('BEGIN')

        # Inserir a consulta na base de dados
        statement_consulta = '''
            INSERT INTO consulta (data_hora, utente_utilizador_id, medico_empregado_utilizador_id) 
            VALUES (%s, %s, %s)
            RETURNING id_consulta
        '''
        values = (appointment_date, d_id, doctor_id)
        cur.execute(statement_consulta, values)

        # ID da consulta criada
        id_consulta = cur.fetchone()[0]

        # Inserir enfermeiros associados à consulta, se houver
        if nurse_ids:
            statement_consulta_enfermeiro = '''
                INSERT INTO enfermeiro_consulta (consulta_id_consulta, enfermeiro_empregado_utilizador_id) 
                VALUES (%s, %s)
            '''
            for nurse_id in nurse_ids:
                cur.execute(statement_consulta_enfermeiro, (id_consulta, nurse_id))

        # Confirmar a transação
        conn.commit()

        # Retornar uma resposta de sucesso
        response = {
            'status': 'success',
            'message': 'Consulta criada com sucesso',
            'id_consulta': id_consulta
        }

        return flask.jsonify(response), StatusCodes['success']

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /appointment - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()
        return flask.jsonify(response), StatusCodes['internal_error']

    finally:
        if conn is not None:
            conn.close()

@app.route('/appointments/<int:patient_user_id>', methods=['GET'])
@jwt_required()
def get_appointments(patient_user_id):
    logger.info('GET /appointments/{}'.format(patient_user_id))
    conn = db_connection()
    cur = conn.cursor()

    try:

        if patient_user_id < 1:
            response = {'status': 'error', 'errors': 'ID inválido'}
            return flask.jsonify(response), StatusCodes['api_error']

        cur.execute("CALL get_patient_appointments(%s, %s::INT, %s::INT ,%s::VARCHAR, %s::VARCHAR, %s::VARCHAR, %s::VARCHAR)", (patient_user_id, None, None,None,None,None,None))
        results = cur.fetchone()


        status = results[4]
        message = results[5]

        if status == 'success':
            appointment_data = {
                'id': results[0],
                'doctor_id': results[1],
                'doctor_name': results[2],
                'date': results[3]
            }

            response = {'status': status, 'appointment': appointment_data}
            return flask.jsonify(response), StatusCodes['success']
        else:
            response = {'status': 'error', 'message': message}
            return flask.jsonify(response), StatusCodes['api_error']

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /appointments/{patient_user_id} - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        return flask.jsonify(response), StatusCodes['internal_error']

    finally:
        if conn is not None:
            conn.close()

@app.route('/surgery', methods=['POST'])
@jwt_required()
def marcar_cirurgia():
    logger.info('POST /surgery')
    assistant_nif = get_jwt_identity()  # NIF do assistente autenticado

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Obter o ID do usuário a partir do NIF
        statement_user_id = 'SELECT id FROM utilizador WHERE nif = %s'
        cur.execute(statement_user_id, (assistant_nif,))
        user_id = cur.fetchone()

        if not user_id:
            response = {'status': 'error', 'message': 'User not found'}
            return flask.jsonify(response), StatusCodes['api_error']

        if is_patient(assistant_nif) or is_medico(assistant_nif):
            response = {'status': 'error', 'message': 'Apenas assistentes podem marcar cirurgias'}
            return flask.jsonify(response), StatusCodes['api_error']

        user_id = user_id[0]

        data = flask.request.get_json()
        patient_id = data.get('patient_id')
        doctor_id = data.get('doctor')
        nurses = data.get('nurses')
        appointment_datetime = data.get('date')
        room_number = data.get('room_number')
        surgery_type = data.get('surgery_type')
        hospitalization_id = data.get('hospitalization_id', None)

        # Verificar campos obrigatórios
        if not all([patient_id, doctor_id, appointment_datetime, room_number, surgery_type]):
            response = {'status': 'error', 'errors': 'Missing required fields'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Verificar se a lista de enfermeiros não está vazia
        if not nurses:
            response = {'status': 'error', 'message': 'At least one nurse is required'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Verificar se o número do quarto já está em uso
        cur.execute('SELECT 1 FROM cirurgia WHERE num_quarto = %s', (room_number,))
        if cur.fetchone():
            response = {'status': 'error', 'message': 'Numero de quarto já em uso'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Extrair o enfermeiro primário da lista de enfermeiros
        primary_nurse = next((nurse_id for nurse_id, role in nurses if role == 'primary'), None)
        if not primary_nurse:
            response = {'status': 'error', 'message': 'Primary nurse not specified'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Verificar se o enfermeiro primário existe
        statement_check_nurse = 'SELECT 1 FROM enfermeiro WHERE empregado_utilizador_id = %s'
        cur.execute(statement_check_nurse, (primary_nurse,))
        if not cur.fetchone():
            response = {'status': 'error', 'message': 'Primary nurse not found'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Verificar ou criar um novo internamento
        if hospitalization_id:
            statement_check_hospitalization = 'SELECT id_internamento FROM internamento WHERE id_internamento = %s'
            cur.execute(statement_check_hospitalization, (hospitalization_id,))
            existing_hospitalization = cur.fetchone()
            if not existing_hospitalization:
                response = {'status': 'error', 'message': 'Hospitalization not found'}
                return flask.jsonify(response), StatusCodes['api_error']
        else:
            statement_create_hospitalization = '''
                INSERT INTO internamento (utente_utilizador_id, data_entrada, num_quarto, enfermeiro_empregado_utilizador_id)
                VALUES (%s, %s, %s, %s)
                RETURNING id_internamento
            '''
            cur.execute(statement_create_hospitalization, (patient_id, appointment_datetime, room_number, primary_nurse))
            hospitalization_id = cur.fetchone()[0]

        # Inserir a cirurgia na base de dados
        statement_insert_surgery = '''
            INSERT INTO cirurgia (tipo, data_hora, num_quarto, utente_utilizador_id, medico_empregado_utilizador_id, internamento_id_internamento)
            VALUES (%s, %s, %s, %s, %s, %s)
            RETURNING id_cirugia, faturacao_id_faturacao
        '''
        values = (surgery_type, appointment_datetime, room_number, patient_id, doctor_id, hospitalization_id)
        cur.execute(statement_insert_surgery, values)
        surgery_id, faturacao_id_faturacao = cur.fetchone()

        # Inserir os enfermeiros relacionados
        for nurse_id, role in nurses:
            statement_insert_nurse = '''
                INSERT INTO enfermeiro_cirurgia (enfermeiro_empregado_utilizador_id, cirurgia_id_cirugia)
                VALUES (%s, %s)
            '''
            cur.execute(statement_insert_nurse, (nurse_id, surgery_id))

        # Commit da transação
        conn.commit()

        # Retornar resposta de sucesso
        response = {
            'status': 'success',
            'message': 'Surgery scheduled successfully',
            'surgery_id': surgery_id,
            'hospitalization_id': hospitalization_id,
            'faturacao_id_faturacao': faturacao_id_faturacao
        }
        return flask.jsonify(response), StatusCodes['success']

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /surgery - error: {error}')
        response = {'status': 'error', 'errors': str(error)}
        conn.rollback()
        return flask.jsonify(response), StatusCodes['internal_error']

    finally:
        if conn is not None:
            conn.close()

@app.route('/prescription', methods=['POST'])
@jwt_required()
def add_prescription():
    # Obter o NIF do usuário autenticado
    doctor_nif = get_jwt_identity()

    # Estabelecer uma conexão com o banco de dados
    conn = db_connection()
    cur = conn.cursor()

    # SÓ OS MEDICOS
    if not is_medico(doctor_nif):
        response = {'status': 'error', 'message': 'Apenas medicos podem criar prescrições'}
        return flask.jsonify(response), 403

    try:

        # Obter o ID do usuário a partir do NIF
        cur.execute('SELECT id FROM utilizador WHERE nif = %s', (doctor_nif,))
        result = cur.fetchone()

        if result is None or len(result) == 0:
            response = {'status': 'error', 'message': 'User not found'}
            return flask.jsonify(response), 404
        user_id = result[0]

        # Verificar se o usuário é um médico
        cur.execute('SELECT 1 FROM medico WHERE empregado_utilizador_id = %s', (user_id,))
        is_doctor = cur.fetchone() is not None

        if not is_doctor:
            response = {'status': 'error', 'message': 'Only doctors can use this endpoint'}
            return flask.jsonify(response), 400

        # Extrair dados do payload da solicitação
        data = flask.request.json

        event_type = data.get('type')
        event_id = data.get('event_id')
        medicines = data.get('medicines')
        prescription_date = data.get('date')  # data da prescrição

        if not all((event_type, event_id, prescription_date, medicines)):
            response = {'status': 'error', 'errors': 'Missing required fields'}
            return flask.jsonify(response), StatusCodes['api_error']

        # Inserir uma nova receita
        if event_type == 'consulta':
            cur.execute('SELECT utente_utilizador_id FROM consulta WHERE id_consulta = %s', (event_id,))
            id_user = cur.fetchone()[0]

            cur.execute('INSERT INTO receita (utente_utilizador_id, data) VALUES (%s, %s) RETURNING id_receita', (id_user, prescription_date))
            prescription_id = cur.fetchone()[0]

            cur.execute('INSERT INTO receita_consulta (receita_id_receita, consulta_id_consulta) VALUES (%s, %s)', (prescription_id, event_id))

            # Inserir detalhes da prescrição na tabela medicamento_dosagem
            for medicine in medicines:
                medicine_name = medicine.get('medicine')
                posology_dose = medicine.get('posology_dose')
                posology_frequency = medicine.get('posology_frequency')
                cur.execute("""
                    INSERT INTO medicamento_dosagem (tipo_medic, dosagem_dosagem, receita_id_receita)
                    VALUES (%s, %s, %s)
                """, (medicine_name, posology_dose, prescription_id))

            # Confirmar a transação
            conn.commit()

            # Retornar resposta de sucesso
            response = {'status': 'success', 'message': 'Prescription added successfully'}
            return flask.jsonify(response), 200

        elif event_type == 'internamento':
            cur.execute('SELECT utente_utilizador_id FROM internamento WHERE id_internamento = %s', (event_id,))
            id_user = cur.fetchone()[0]

            cur.execute('INSERT INTO receita (utente_utilizador_id, data) VALUES (%s, %s) RETURNING id_receita', (id_user, prescription_date))
            prescription_id = cur.fetchone()[0]

            cur.execute('INSERT INTO internamento_receita (receita_id_receita, internamento_id_internamento) VALUES (%s, %s)', (prescription_id, event_id))

            # Inserir detalhes da prescrição na tabela medicamento_dosagem
            for medicine in medicines:
                medicine_name = medicine.get('medicine')
                posology_dose = medicine.get('posology_dose')
                posology_frequency = medicine.get('posology_frequency')
                cur.execute("""
                    INSERT INTO medicamento_dosagem (tipo_medic, dosagem_dosagem, receita_id_receita)
                    VALUES (%s, %s, %s)
                """, (medicine_name, posology_dose, prescription_id))

            # Confirmar a transação
            conn.commit()

            # Retornar resposta de sucesso
            response = {'status': 'success', 'message': 'Prescription added successfully'}
            return flask.jsonify(response), 200

        else:
            response = {'status': 'error', 'message': 'Wrong type'}
            return flask.jsonify(response), 400

    except Exception as e:
        conn.rollback()
        response = {'status': 'error', 'message': str(e)}
        return flask.jsonify(response), 500

    finally:
        if conn is not None:
            conn.close()

@app.route('/prescriptions/<int:person_id>', methods=['GET'])
@jwt_required()
def get_prescriptions(person_id):
    conn = db_connection()
    cur = conn.cursor()

    user_nif = get_jwt_identity()

    try:

        # Obter o nif do usuário da prescrição
        cur.execute('SELECT nif FROM utilizador WHERE id = %s', (person_id,))
        nif_user_prescricao = cur.fetchone()

        if nif_user_prescricao is None or len(nif_user_prescricao) == 0:
            response = {'status': 'error', 'message': 'User not found'}
            return flask.jsonify(response), 404
        user_id = nif_user_prescricao[0]

        if (is_medico(user_nif)):
            response = {'status': 'error', 'message': 'Só pacientes e assistentes é que podem consultar'}
            return flask.jsonify(response), 404

        # verificar se e paciente
        if (is_patient(user_nif) ):
            # se o id do paciente autenticado for diferente do id do paciente da prescricao alvo
            print(user_nif)
            print(user_id)
            if user_nif != user_id:
                response = {'status': 'error', 'message': 'Não tem acesso a esta prescrição'}
                return flask.jsonify(response), 404



        # Chamar o procedimento armazenado
        cur.execute(
            "CALL get_prescriptions(%s,  %s::VARCHAR, %s::VARCHAR)",
            (person_id, None, None))

        # Recuperar os resultados
        result = cur.fetchall()

        # Verificar se há resultados
        if result:
            status = result[0][0]  # O status está na primeira posição do resultado
            prescriptions_json = result[0][1]  # Os dados das prescrições estão na segunda posição

            if status == 'success':
                prescriptions = flask.json.loads(prescriptions_json)
                response = {'status': status, 'prescriptions': prescriptions}
                return flask.jsonify(response), 200
            else:
                response = {'status': 'error', 'message': prescriptions_json}
                return flask.jsonify(response), 404
        else:
            response = {'status': 'error', 'message': 'No data found'}
            return flask.jsonify(response), 404

    except Exception as e:
        conn.rollback()
        response = {'status': 'error', 'message': str(e)}
        return flask.jsonify(response), 500

    finally:
        if conn is not None:
            conn.close()



@app.route('/bills', methods=['POST'])
@jwt_required()
def process_payment():

    user_nif = get_jwt_identity()
    conn = db_connection()
    cur = conn.cursor()

    statement_user_info = 'SELECT id FROM utilizador WHERE nif = %s'
    cur.execute(statement_user_info, (user_nif,))
    user_info = cur.fetchone()
    user_id = user_info[0]

    cur.execute('SELECT 1 FROM utilizador WHERE id = %s', (user_id,))
    is_utente = cur.fetchone() is not None
    if not is_utente:
        return flask.jsonify({"error": "Usuário autenticado não é um paciente."}), 403

    dados_pagamento = flask.request.json
    conta_id = dados_pagamento.get('conta_id')
    valor_pagamento = dados_pagamento.get('valor')

    cur.execute('''
        SELECT 1 FROM cirurgia WHERE faturacao_id_faturacao = %s AND utente_utilizador_id = %s
        UNION ALL
        SELECT 1 FROM consulta WHERE faturacao_id_faturacao = %s AND utente_utilizador_id = %s
        ''', (conta_id, user_id, conta_id, user_id))
    is_conta_paciente = cur.fetchone() is not None

    if is_conta_paciente:
        cur.execute('SELECT valor_pago from pagamento WHERE faturacao_id_faturacao = %s',
                    (conta_id,))
        valor = cur.fetchone()[0]
        if valor:
            valor_pagamento = valor_pagamento + valor

        cur.execute('UPDATE pagamento SET valor_pago = %s WHERE faturacao_id_faturacao = %s',
                    (valor_pagamento, conta_id))

        cur.execute('SELECT preco FROM pagamento WHERE faturacao_id_faturacao = %s', (conta_id,))
        preco_total = cur.fetchone()[0]

        cur.execute('SELECT SUM(valor_pago) FROM pagamento WHERE faturacao_id_faturacao = %s', (conta_id,))
        valor_total_pago = cur.fetchone()[0]

        if valor_total_pago >= preco_total:
            cur.execute('UPDATE pagamento SET estado_pag = %s WHERE faturacao_id_faturacao = %s', ('paid', conta_id))
        conn.commit()
        return flask.jsonify({"message": "Pagamento concluído com sucesso."}), 200
    else:
        return flask.jsonify({"error": "Não tem acesso a esta conta."}), 403




def get_mes_atual():
    now = datetime.now()
    mes_atual = now.month
    ano_atual = now.year

    return mes_atual, ano_atual

@app.route('/top3', methods=['GET'])
@jwt_required()
def top_3():
    # Obter o NIF do usuário autenticado
    user_nif = get_jwt_identity()

    # Estabelecer uma conexão com a base de dados
    conn = db_connection()
    cur = conn.cursor()
    dicionario = {
        '1': {'preço_total': 0, 'id_utente': 0},
        '2': {'preço_total': 0, 'id_utente': 0},
        '3': {'preço_total': 0, 'id_utente': 0}
    }

    mes_atual, ano_atual = get_mes_atual()
    try:

        if (is_patient(user_nif)) or (is_medico(user_nif)):
            response = {'status': 'error', 'message': 'Apenas assistentes podem consultar o top 3'}
            return flask.jsonify(response), 403


        # Obter o ID do usuário a partir do NIF
        cur.execute('SELECT utilizador_id FROM utente')
        todosUtentes_id = cur.fetchall()
        todosUtentes_id = [item[0] for item in todosUtentes_id]
        print(todosUtentes_id)
        if not todosUtentes_id:
            response = {'status': 'error', 'message': 'User not found'}
            return flask.jsonify(response), 404

        for utente_atual in todosUtentes_id:
            cur.execute('SELECT faturacao_id_faturacao FROM consulta WHERE utente_utilizador_id = %s ', (utente_atual,))
            id_faturas_consultas = cur.fetchall()
            id_faturas_consultas = [item[0] for item in id_faturas_consultas]

            cur.execute('SELECT faturacao_id_faturacao FROM cirurgia WHERE utente_utilizador_id = %s ', (utente_atual,))
            id_faturas_cirurgias = cur.fetchall()
            id_faturas_cirurgias = [item[0] for item in id_faturas_cirurgias]

            # Unir as listas de faturas de consultas e cirurgias
            id_faturas = id_faturas_consultas + id_faturas_cirurgias

            print(id_faturas)
            preco_total = 0
            for id_fatura_atual in id_faturas:
                cur.execute('''
                        SELECT valor_pago
                        FROM pagamento
                        WHERE faturacao_id_faturacao = %s AND EXTRACT(MONTH FROM data_pag) = %s ''', (id_fatura_atual, mes_atual))
                preco_atual = cur.fetchone()
                print(id_fatura_atual)
                print(preco_atual)
                if preco_atual[0]:
                    preco_total += preco_atual[0]

            # Atualizar o dicionário principal com os top 3 preços
            preco_total_1 = dicionario['1']['preço_total']
            preco_total_2 = dicionario['2']['preço_total']
            preco_total_3 = dicionario['3']['preço_total']

            if preco_total > preco_total_1:
                dicionario['3'] = dicionario['2']
                dicionario['2'] = dicionario['1']
                dicionario['1'] = {'preço_total': preco_total, 'id_utente': utente_atual}
            elif preco_total > preco_total_2:
                dicionario['3'] = dicionario['2']
                dicionario['2'] = {'preço_total': preco_total, 'id_utente': utente_atual}
            elif preco_total > preco_total_3:
                dicionario['3'] = {'preço_total': preco_total, 'id_utente': utente_atual}


        # Extrair nomes e procedimentos dos top 3 utentes depois de ter acabdo o ciclo for que iterou sobre todos os utentes

        results = []
        for rank in ['1', '2', '3']:
            id_utente = dicionario[rank]['id_utente']
            preco_pago = dicionario[rank]['preço_total']

            cur.execute('SELECT nome FROM utilizador WHERE id = %s', (id_utente,))
            nome_utente = cur.fetchone()
            nome_utente = nome_utente[0] if nome_utente else None

            resultados_procedimentos = {
                'consultas': [],
                'cirurgias': []
            }

            # Consultas
            cur.execute(
                'SELECT id_consulta, data_hora, medico_empregado_utilizador_id, faturacao_id_faturacao FROM consulta WHERE utente_utilizador_id = %s',
                (id_utente,))
            consultas = cur.fetchall()
            for consulta in consultas:
                id_consulta, data_hora, id_medico, faturacao_id = consulta
                cur.execute('SELECT valor_pago FROM pagamento WHERE faturacao_id_faturacao = %s', (faturacao_id,))
                preco = cur.fetchone()
                preco = preco[0] if preco else None
                resultados_procedimentos['consultas'].append({
                    'id_consulta': id_consulta,
                    'data_hora': data_hora,
                    'id_medico': id_medico,
                    'valor pago': preco
                })

            # Cirurgias
            cur.execute(
                'SELECT id_cirugia, data_hora, medico_empregado_utilizador_id, tipo, faturacao_id_faturacao FROM cirurgia WHERE utente_utilizador_id = %s',
                (id_utente,))
            cirurgias = cur.fetchall()
            for cirurgia in cirurgias:
                id_cirurgia, data_hora, id_enfermeiro, tipo, faturacao_id = cirurgia
                cur.execute('SELECT valor_pago FROM pagamento WHERE faturacao_id_faturacao = %s', (faturacao_id,))
                preco = cur.fetchone()
                preco = preco[0] if preco else None
                resultados_procedimentos['cirurgias'].append({
                    'id_cirurgia': id_cirurgia,
                    'data_hora': data_hora,
                    'id_enfermeiro': id_enfermeiro,
                    'tipo': tipo,
                    'valor pago': preco
                })

            results.append({
                'rank': rank,
                'nome_utente': nome_utente,
                'preco_pago': preco_pago,
                'procedimentos': resultados_procedimentos
            })

        response = {'status': 'success', 'results': results}
        return flask.jsonify(response), 200

    except Exception as e:
        conn.rollback()
        response = {'status': 'error', 'message': str(e)}
        return flask.jsonify(response), 500

    finally:
        if conn is not None:
            conn.close()


@app.route('/daily/<date>', methods=['GET'])
@jwt_required()
def get_statistics(date):

    current_user_nif = get_jwt_identity()

    conn = db_connection()
    cur = conn.cursor()

    try:

        if not is_assistant(current_user_nif):
            response = {'status': 'error', 'message': 'Apenas assistentes podem consultar o sumário diário'}
            return flask.jsonify(response), 403

        # Consulta para as cirurgias
        cur.execute("""
            SELECT 'Surgeries' AS category, COUNT(*) AS count
            FROM cirurgia
            WHERE DATE(data_hora) = %s
        """, (date,))
        surgeries_count = cur.fetchone()

        # Consulta para os pagamentos
        cur.execute("""
            SELECT 'Payments' AS category, COUNT(*) AS count
            FROM pagamento
            WHERE DATE(data_pag) = %s
        """, (date,))
        payments_count = cur.fetchone()

        # Consulta para o valor total gasto
        cur.execute("""
            SELECT SUM(valor_pago) AS total_amount
            FROM pagamento
            WHERE DATE(data_pag) = %s
        """, (date,))
        total_amount = cur.fetchone()[0]

        # Retorna a resposta
        response = {
            'surgeries_count': surgeries_count[1],
            'payments_count': payments_count[1],
            'amount_spent': total_amount
        }
        return flask.jsonify(response), 200

    except (Exception, psycopg2.DatabaseError) as error:
        response = {'error': str(error)}
        return flask.jsonify(response), 500

    finally:
        if conn is not None:
            conn.close()


@app.route('/report', methods=['GET'])
@jwt_required()
def report():


    current_user_nif = get_jwt_identity()
    # Estabelecer uma conexão com a base de dados
    conn = db_connection()
    cur = conn.cursor()
    mes_atual, ano_atual = get_mes_atual()
    mes_aux = mes_atual
    mes = mes_atual
    ano_aux = ano_atual

    dicionario = {
        1: {'id_medico': 0, 'cirurgias': 0},
        2: {'id_medico': 0, 'cirurgias': 0},
        3: {'id_medico': 0, 'cirurgias': 0},
        4: {'id_medico': 0, 'cirurgias': 0},
        5: {'id_medico': 0, 'cirurgias': 0},
        6: {'id_medico': 0, 'cirurgias': 0},
        7: {'id_medico': 0, 'cirurgias': 0},
        8: {'id_medico': 0, 'cirurgias': 0},
        9: {'id_medico': 0, 'cirurgias': 0},
        10: {'id_medico': 0, 'cirurgias': 0},
        11: {'id_medico': 0, 'cirurgias': 0},
        12: {'id_medico': 0, 'cirurgias': 0}
    }

    chave_atual = 1
    try:

        if not is_assistant(current_user_nif):
            response = {'status': 'error', 'message': 'Apenas assistentes podem consultar as estatísticas dos ultimos 12 meses'}
            return flask.jsonify(response), 403

        cur.execute('SELECT empregado_utilizador_id FROM medico')
        todosMedicos_id = cur.fetchall()
        todosMedicos_id = [medico[0] for medico in todosMedicos_id] if todosMedicos_id else []

        while mes_atual > 0:
            for medico_atual in todosMedicos_id:
                cur.execute(
                    'SELECT COUNT(id_cirugia) FROM cirurgia WHERE medico_empregado_utilizador_id= %s AND EXTRACT(MONTH FROM data_hora) = %s AND EXTRACT(YEAR FROM data_hora) = %s',
                    (medico_atual, mes_atual, ano_atual))
                num_cir = cur.fetchone()[0]

                if num_cir is not None:
                    if num_cir > dicionario[chave_atual]['cirurgias']:
                        dicionario[chave_atual]['cirurgias'] = num_cir
                        dicionario[chave_atual]['id_medico'] = medico_atual

            chave_atual += 1
            mes_atual -= 1


        ano_atual -=1
        mes_fim = 12
        while(chave_atual<12):
            for medico_atual in todosMedicos_id:
                cur.execute(
                    'SELECT COUNT(id_cirugia) FROM cirurgia WHERE medico_empregado_utilizador_id= %s AND EXTRACT(MONTH FROM data_hora) = %s AND EXTRACT(YEAR FROM data_hora) = %s',
                    (medico_atual, mes_fim, ano_atual))
                num_cir = cur.fetchone()[0]

                if num_cir is not None:
                    if num_cir > dicionario[chave_atual]['cirurgias']:
                        dicionario[chave_atual]['cirurgias'] = num_cir
                        dicionario[chave_atual]['id_medico'] = medico_atual
            chave_atual += 1
            mes_aux -= 1

        results = []

        for chave, valor in dicionario.items():
            id_medico = valor['id_medico']
            num_cirurgias = valor['cirurgias']
            result_item = {'month': chave, 'doctor': id_medico, 'surgeries': num_cirurgias}
            results.append(result_item)

        response = {'status': 'success', 'results': results}
        return flask.jsonify(response), 200

    except Exception as e:
        conn.rollback()
        response = {'status': 'error', 'message': str(e)}
        return flask.jsonify(response), 500

    finally:
        if conn is not None:
            conn.close()

if __name__ == '__main__':

    # set up logging
    logging.basicConfig(filename='log_file.log')
    logger = logging.getLogger('logger')
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)

    # create formatter
    formatter = logging.Formatter('%(asctime)s [%(levelname)s]:  %(message)s', '%H:%M:%S')
    ch.setFormatter(formatter)
    logger.addHandler(ch)

    host = '127.0.0.1'
    port = 8080
    app.run(host=host, debug=True, threaded=True, port=port)
    logger.info(f'API v1.0 online: http://{host}:{port}')