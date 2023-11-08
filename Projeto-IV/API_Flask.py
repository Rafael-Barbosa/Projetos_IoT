from flask import Flask, request, jsonify
import psycopg2

app = Flask(__name__)

db_host = "localhost"
db_database = "Projetos_IoT"
db_user = "postgres"
db_password = "postgres"

conn = psycopg2.connect(
    host=db_host,
    database=db_database,
    user=db_user,
    password=db_password
)

@app.route('/get-sensor-data', methods=['GET'])
def post_data():
    print("Requisição recebida")
    temperature = request.args.get('temperature')
    humidity = request.args.get('humidity')

    if temperature is not None and humidity is not None:
        try:
            temperature = float(temperature)
            humidity = float(humidity)

            cur = conn.cursor()
            cur.execute("INSERT INTO sensor_data (temperature, humidity) VALUES (%s, %s)", (temperature, humidity))
            conn.commit()
            cur.close()

            return jsonify({'success': True}), 200
        except (ValueError, psycopg2.DatabaseError) as e:
            return jsonify({'error': 'Bad Request', 'message': str(e)}), 400
    else:
        return jsonify({'error': 'Missing data', 'message': 'Temperature and humidity required'}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
    
 
