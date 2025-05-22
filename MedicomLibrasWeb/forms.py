# Formulário médico
class MedicalForm(FlaskForm):
    name = StringField('Nome Completo', validators=[DataRequired()])
    blood_type = SelectField('Tipo Sanguíneo', choices=[('A+', 'A+'), ('A-', 'A-'), ('B+', 'B+'), ('B-', 'B-'), ('O+', 'O+'), ('O-', 'O-'), ('AB+', 'AB+'), ('AB-', 'AB-')])
    birth_date = DateField('Data de Nascimento', format='%Y-%m-%d', validators=[DataRequired()])
    rg = StringField('RG', validators=[DataRequired()])
    cpf = StringField('CPF', validators=[DataRequired()])
    sus_number = StringField('Número do Cartão SUS', validators=[DataRequired()])
    cid = StringField('CID', validators=[DataRequired()])
    medication = SelectField('Toma algum remédio controlado?',
        choices=[
            ('', 'Selecione uma opção'),
            ('sim', 'Sim'),
            ('nao', 'Não')
        ],
        validators=[DataRequired(message='Por favor, selecione uma opção')]
    )
    # Outros campos omitidos para brevidade
    submit = SubmitField('Gerar Cartão')
