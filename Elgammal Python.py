import base64
import math

def E_kiemTraNguyenTo(so_kt: int) -> bool:
    if so_kt <= 1: return False
    if so_kt <= 3: return True
    if so_kt % 2 == 0 or so_kt % 3 == 0: return False
    for i in range(5, int(math.sqrt(so_kt)) + 1, 6):
        if so_kt % i == 0 or so_kt % (i + 2) == 0:
            return False
    return True

def nguyenToCungNhau(ai: int, bi: int) -> bool:
    while bi != 0:
        ai, bi = bi, ai % bi
    return ai == 1

def E_LuyThuaModulo_(a: int, b: int, n: int) -> int:
    f = 1
    a = a % n
    while b > 0:
        if b % 2 == 1:
            f = (f * a) % n
        b = b // 2
        a = (a * a) % n
    return f

EsoP = 2357
EsoA = 0
EsoX = 0
EsoD = 0
EsoK = 0

txt_So_C1 = ""
txt_So_C2 = ""


def E_MaHoa(du_lieu_vao: str, k_input: int) -> str:
    global EsoA, EsoP, EsoD, txt_So_C1, txt_So_C2
    if du_lieu_vao.strip().isdigit() and int(du_lieu_vao) < EsoP:
        M_so = int(du_lieu_vao)
        C1 = E_LuyThuaModulo_(EsoA, k_input, EsoP)
        C2 = (M_so * E_LuyThuaModulo_(EsoD, k_input, EsoP)) % EsoP
        txt_So_C1 = str(C1)
        txt_So_C2 = str(C2)
        return f"SO:{C1},{C2}"
    else:
        mhE_temp1 = du_lieu_vao.encode('utf-16-le')
        base64_str = base64.b64encode(mhE_temp1).decode('utf-8')
        mh_temp2 = [ord(char) for char in base64_str]
        C1_co_dinh = E_LuyThuaModulo_(EsoA, k_input, EsoP)
        beta_k = E_LuyThuaModulo_(EsoD, k_input, EsoP)
        C2 = [0] * len(mh_temp2)
        so_c1 = ""
        so_c2 = ""
        for i in range(len(mh_temp2)):
            C2[i] = (mh_temp2[i] * beta_k) % EsoP
            so_c1 += str(C1_co_dinh) + " "
            so_c2 += str(C2[i]) + " "
        txt_So_C1 = so_c1.strip()
        txt_So_C2 = so_c2.strip()
        str_builder = [f"{C1_co_dinh},{C2[i]};" for i in range(len(C2))]
        full_pair_str = "".join(str_builder)
        return "CHUOI:" + base64.b64encode(full_pair_str.encode('utf-16-le')).decode('utf-8')

def E_GiaiMa(chuoi_ma_hoa: str) -> str:
    global EsoP, EsoX
    if chuoi_ma_hoa.startswith("SO:"):
        ban_ma_sach = chuoi_ma_hoa.replace("SO:", "")
        pair = ban_ma_sach.split(',')
        C1 = int(pair[0])
        C2 = int(pair[1])
        s = E_LuyThuaModulo_(C1, (EsoP - 1 - EsoX), EsoP)
        M_giai_ma = (C2 * s) % EsoP
        return f"[Dạng Số] {M_giai_ma}"
    elif chuoi_ma_hoa.startswith("CHUOI:"):
        ban_ma_sach = chuoi_ma_hoa.replace("CHUOI:", "")
        E_data1 = base64.b64decode(ban_ma_sach.encode('utf-8'))
        Egm_giaima = E_data1.decode('utf-16-le')
        pairs = [p for p in Egm_giaima.split(';') if p]
        length = len(pairs)
        C1 = [0] * length
        C2 = [0] * length
        for i in range(length):
            pair = pairs[i].split(',')
            C1[i] = int(pair[0])
            C2[i] = int(pair[1])
        M = [0] * length
        for i in range(length):
            s = E_LuyThuaModulo_(C1[i], (EsoP - 1 - EsoX), EsoP)
            M[i] = (C2[i] * s) % EsoP
        str_builder = [chr(M[i]) for i in range(length)]
        base64_original = "".join(str_builder)
        van_ban_goc = base64.b64decode(base64_original.encode('utf-8')).decode('utf-16-le')
        return f"[Dạng Chuỗi] {van_ban_goc}"
    return "[LỖI] Không nhận diện được định dạng bản mã!"

if _name_ == "_main_":
    print("====================================================")
    print("   HỆ THỐNG MẬT MÃ ELGAMAL TỰ ĐỘNG NHẬN DIỆN 100%   ")
    print("====================================================")
    while True:
        EsoP = int(input("1. Nhập số nguyên tố P : "))
        if E_kiemTraNguyenTo(EsoP):
            break
        else:
            print(f"[LỖI] Số {EsoP} không phải là số nguyên tố! Vui lòng nhập lại.\n")
    while True:
        EsoA = int(input("2. Nhập phần tử sinh alpha A : "))
        if EsoA >= EsoP:
            print(f"[LỖI] Phần tử sinh alpha A phải nhỏ hơn P ({EsoP})! Vui lòng nhập lại.\n")
        elif not nguyenToCungNhau(EsoA, EsoP):
            print(f"[LỖI] Alpha A ({EsoA}) và P ({EsoP}) phải nguyên tố cùng nhau! Vui lòng nhập lại.\n")
        else:
            break
    EsoX = int(input("3. Nhập khóa bí mật XA : "))
    EsoD = E_LuyThuaModulo_(EsoA, EsoX, EsoP)
    print("\n----------------------------------------------------")
    print(f" * KHÓA CÔNG KHAI : (P = {EsoP}, alpha = {EsoA}, YA = {EsoD})")
    print(f" * KHÓA BÍ MẬT   : XA = {EsoX}")
    print("----------------------------------------------------\n")
    du_lieu_nhap = input("-> Nhập dữ liệu cần mã hóa (Chuỗi hoặc Số đều được): ")
    EsoK = int(input("-> Nhập số K để tính toán : "))
    ban_ma_packet = E_MaHoa(du_lieu_nhap, EsoK)
    print("\n--- KẾT QUẢ MÃ HÓA TỰ ĐỘNG ---")
    print(f"Số k đã dùng: {EsoK}")
    print(f"Gói bản mã xuất ra: {ban_ma_packet}")
    print(f"Mảng số C1: {txt_So_C1}")
    print(f"Mảng số C2: {txt_So_C2}")
    ket_qua_giai_ma = E_GiaiMa(ban_ma_packet)
    print("\n--- KẾT QUẢ GIẢI MÃ TỰ ĐỘNG ---")
    print(f"Kết quả sau khi giải mã: {ket_qua_giai_ma}")
    print("====================================================")
