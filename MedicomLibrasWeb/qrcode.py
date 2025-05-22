@app.route('/generate_qrcode')
@login_required
def generate_qrcode():
    # Buscar o registro médico do usuário atual
    record = MedicalRecord.query.filter_by(user_id=current_user.id).first()
    
    if not record:
        # Se não houver registro, retornar um QR code com mensagem de erro
        data = "Registro médico não encontrado"
    else:
        # Formatar a data de nascimento
        birth_date_formatted = record.birth_date.strftime('%d/%m/%Y')
        
        # Criar string com informações
        data = (
            f"CARTÃO MÉDICO VIRTUAL\n\n"
            f"Nome: {record.name}\n"
            f"Data de Nascimento: {birth_date_formatted}\n"
            f"Tipo Sanguíneo: {record.blood_type}\n"
            f"CPF: {record.cpf}\n"
            f"Cartão SUS: {record.sus_number}\n"
            f"CID: {record.cid}\n\n"
            f"INFORMAÇÕES MÉDICAS\n"
            f"Alergias: {record.allergies}\n"
            f"Doença Crônica: {record.chronic_disease}\n"
            f"Medicação Controlada: {record.medication}\n"
            f"Histórico Cirúrgico: {record.surgery}\n"
            f"Doenças na Família: {record.family_diseases}"
        )
    
    # Configurar e gerar QR code
    qr = qrcode.QRCode(
        version=None,
        error_correction=qrcode.constants.ERROR_CORRECT_Q,
        box_size=10,
        border=4,
    )
    qr.add_data(data)
    qr.make(fit=True)
    
    # Criar e retornar imagem do QR code
    img = qr.make_image(fill_color="black", back_color="white")
    buf = BytesIO()
    img.save(buf, format='PNG')
    buf.seek(0)
    return send_file(buf, mimetype='image/png')
