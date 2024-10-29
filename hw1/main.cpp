#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    //计算弧度
    float angle = rotation_angle / 180.0 * MY_PI;
    model(0,0)= std::cos(angle);
    model(0,1)=-std::sin(angle);
    model(1,0)=std::sin(angle);
    model(1,1)=std::cos(angle);
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

//    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    //计算弧度
    float fovY=eye_fov/180.0*MY_PI;

    //计算t,b,r,l
    float yTop=std::abs(zNear)*std::tan(fovY/2);
    float yBottom=-yTop;
    float xRight=aspect_ratio*yTop;
    float xLeft=-xRight;

    // 计算正交投影矩阵
    Eigen::Matrix4f M_orthographic_translation=Eigen::Matrix4f::Identity();
    M_orthographic_translation(0,3)=-(xRight+xLeft)/2;
    M_orthographic_translation(1,3)=-(yTop+yBottom)/2;
    M_orthographic_translation(2,3)=-(zNear+zFar)/2;
    Eigen::Matrix4f M_orthographic_scale=Eigen::Matrix4f::Identity();
    M_orthographic_scale(0,0)=2/(xRight-xLeft);
    M_orthographic_scale(1,1)=2/(yTop-yBottom);
    M_orthographic_scale(2,2)=2/(zNear-zFar);
    Eigen::Matrix4f M_orthographic= M_orthographic_scale*M_orthographic_translation;

    //计算透视投影到正交投影的变换矩阵
    Eigen::Matrix4f M_perspective_to_orthographic=Eigen::Matrix4f::Zero();
    M_perspective_to_orthographic(0,0)=zNear;
    M_perspective_to_orthographic(1,1)=zNear;
    M_perspective_to_orthographic(2,2)=zNear+zFar;
    M_perspective_to_orthographic(2,3)=-zNear*zFar;
    M_perspective_to_orthographic(3,2)=1;

    //计算透视投影矩阵M_{persp}
    Eigen::Matrix4f projection=M_orthographic*M_perspective_to_orthographic;
    return projection;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle)
{
    // 得到绕任意过原点的轴的旋转变换矩阵
    
    //先保证方向向量是单位向量
    axis.normalize();
    // 使用弧度
    float angle_rad = angle / 180.0 * MY_PI;

    // 使用方向向量填充反对称矩阵
    Eigen::Matrix3f N;
    N << 0, -axis.z(), axis.y(),
         axis.z(), 0, -axis.x(),
         -axis.y(), axis.x(), 0;

    // 旋转矩阵：Rodrigues' rotation formula
    Eigen::Matrix3f R=std::cos(angle_rad)*Eigen::Matrix3f::Identity()+
                    (1-std::cos(angle_rad))*axis*axis.transpose()+
                    std::sin(angle_rad)* N;

    // 4x4矩阵
    Eigen::Matrix4f R4=Eigen::Matrix4f::Identity();
    R4.block(0,0,3,3)=R;

    return R4;
}

Eigen::Vector3f get_axis(char axis_char){
    if (axis_char=='x'){
        return Vector3f(1,0,0);
    }else if (axis_char=='y'){
        return Vector3f(0,1,0);
    }else if (axis_char=='z'){
        return Vector3f(0,0,1);
    }else {
        std::cout<<"axis error"<<std::endl;
        return Vector3f(1,1,1);
    }
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    char axis_char='z';
    Eigen::Matrix4f rotation_matrix = get_rotation(get_axis(axis_char),angle);
    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(rotation_matrix);
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(76, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);


        if (key == 'a') {
            // angle += 10;
            rotation_matrix=rotation_matrix*get_rotation(get_axis(axis_char),11);
            //输出绕哪个轴旋转了多少度
            std::cout << "rotate around " << axis_char << " axis by 11 degrees" << std::endl;
        }
        else if (key == 'd') {
            // angle -= 10;
            rotation_matrix=rotation_matrix*get_rotation(get_axis(axis_char),-11);
            std::cout << "rotate around " << axis_char << " axis by 11 degrees" << std::endl;
        }else if (key == 'u'){
            //按u切换旋转轴
            switch (axis_char) {
                case 'x':
                    axis_char='y';
                    break;
                case 'y':
                    axis_char='z';
                    break;
                case 'z':
                    axis_char='x';
                    break;
                default:
                    break;
            }
            std::cout<<"change axis to "<<axis_char<<std::endl;
        }
    }

    return 0;
}
