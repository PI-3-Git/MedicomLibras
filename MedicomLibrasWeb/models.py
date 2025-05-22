
# Modelo de usuário
class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(150), unique=True, nullable=False)
    password = db.Column(db.String(150), nullable=False)

# Modelo de registro médico
class MedicalRecord(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    name = db.Column(db.String(150), nullable=False)
    blood_type = db.Column(db.String(3), nullable=False)
    birth_date = db.Column(db.Date, nullable=False)
    rg = db.Column(db.String(20), nullable=False)
    cpf = db.Column(db.String(20), nullable=False)
    sus_number = db.Column(db.String(20), nullable=False)
    cid = db.Column(db.String(10), nullable=False)
    medication = db.Column(db.Text, nullable=False)
    surgery = db.Column(db.Text, nullable=False)
    family_diseases = db.Column(db.Text, nullable=False)
    allergies = db.Column(db.Text, nullable=False)
    chronic_disease = db.Column(db.Text, nullable=False)

    user = db.relationship('User', backref='medical_records')
