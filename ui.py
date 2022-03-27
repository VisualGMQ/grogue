import pygame
import pygame.gfxdraw
import enum
import mouse
import copy
import global_var


class ButtonType(enum.IntEnum):
    TriangleLeft = 0
    TriangleRight = 1
    Rect = 2


class ButtonEvent(enum.IntEnum):
    Unknown = 0,
    Click = 1,
    Hover = 2,


def Button(surface: pygame.Surface, color: pygame.Color, btn_type: ButtonType, rect: pygame.Rect) -> ButtonEvent:
    if btn_type == ButtonType.TriangleLeft:
        return __draw_left_triangle_button(surface, color, rect)
    elif btn_type == ButtonType.TriangleRight:
        return __draw_right_triangle_button(surface, color, rect)
    elif btn_type == ButtonType.Rect:
        return __draw_rect_button(surface, color, rect)


def __draw_left_triangle_button(surface: pygame.Surface, color: pygame.Color, rect: pygame.Rect) -> ButtonEvent:
    p1 = pygame.math.Vector2(rect.x, rect.y + rect.h // 2)
    p2 = pygame.math.Vector2(rect.x + rect.w, rect.y)
    p3 = pygame.math.Vector2(rect.x + rect.w, rect.y + rect.h)
    return __draw_triangle_and_judge(surface, color, (p1, p2, p3))


def __draw_right_triangle_button(surface: pygame.Surface, color: pygame.Color, rect: pygame.Rect) -> ButtonEvent:
    p1 = pygame.math.Vector2(rect.x, rect.y)
    p2 = pygame.math.Vector2(rect.x, rect.y + rect.h)
    p3 = pygame.math.Vector2(rect.x + rect.w, rect.y + rect.h // 2)
    return __draw_triangle_and_judge(surface, color, (p1, p2, p3))


def __draw_triangle_and_judge(surface: pygame.Surface, color: pygame.Color, triangle: (pygame.math.Vector2)) -> ButtonEvent:
    event = ButtonEvent.Unknown
    if is_point_in_triangle(mouse.Mouse.get_pos(), triangle):
        if mouse.Mouse.is_left_pressed():
            event = ButtonEvent.Click
        else:
            event = ButtonEvent.Hover

    drawcolor = copy.deepcopy(copy)
    if event == ButtonEvent.Hover:
        drawcolor.r *= 0.1
        drawcolor.g *= 0.1
        drawcolor.b *= 0.1

    pygame.gfxdraw.filled_trigon(surface,
                                 int(triangle[0].x), int(triangle[0].y),
                                 int(triangle[1].x), int(triangle[1].y),
                                 int(triangle[2].x), int(triangle[2].y),
                                 drawcolor)

    pygame.gfxdraw.trigon(surface,
                          int(triangle[0].x), int(triangle[0].y),
                          int(triangle[1].x), int(triangle[1].y),
                          int(triangle[2].x), int(triangle[2].y),
                          (0, 0, 0, 255))
    return event


def is_point_in_triangle(point: pygame.math.Vector2, triangle: (pygame.math.Vector2)) -> bool:
    v1 = triangle[1] - triangle[0]
    v2 = triangle[2] - triangle[1]
    v3 = triangle[0] - triangle[2]

    c1 = (triangle[0] - point).cross(v1)
    c2 = (triangle[1] - point).cross(v2)
    c3 = (triangle[2] - point).cross(v3)
    return (c1 > 0 and c2 > 0 and c3 > 0) or (c1 <= 0 and c2 <= 0 and c3 <= 0)


def __draw_rect_button(surface: pygame.Surface, color: pygame.Color, rect: pygame.Rect) -> ButtonEvent:
    event = ButtonEvent.Unknown

    if rect.collidepoint(int(mouse.Mouse.get_pos().x), int(mouse.Mouse.get_pos().y)):
        if mouse.Mouse.is_left_pressed():
            event = ButtonEvent.Click
        else:
            event = ButtonEvent.Hover

    drawcolor = copy.deepcopy(color)
    if event == ButtonEvent.Hover:
        drawcolor.update(int(color.r * 0.1), int(color.g * 0.1), int(color.b * 0.1))
    pygame.draw.rect(surface, drawcolor, rect)
    pygame.draw.rect(surface, (255, 255, 255, 255), rect, 1)
    return event


class ItemDisplayInfo:
    def __init__(self, image: pygame.Surface.subsurface, description: str):
        self.image = image
        self.description = description


# list all items in a panel. Return the clicked item index in list
def ItemDisplayPanel(surface: pygame.Surface, rect: pygame.Rect, image_size: int, items: list) -> int:
    border_radius = 10
    pygame.draw.rect(surface,
                     (71, 71, 71, 100),
                     rect,
                     0,
                     border_radius, border_radius, border_radius, border_radius)
    padding = 8
    x_item_num = rect.w // (image_size + padding)
    y_item_num = rect.h // (image_size + padding)

    for i in range(len(items)):
        offset_x = i % x_item_num
        offset_x = offset_x * image_size + (offset_x + 1) * padding
        offset_y = i // x_item_num
        offset_y = offset_y * image_size + (offset_y + 1) * padding

        event = Button(surface,
                       pygame.Color(60, 60, 60),
                       ButtonType.Rect,
                       pygame.Rect(offset_x + rect.x, offset_y + rect.y,
                                   image_size, image_size))

        item: ItemDisplayInfo = items[i]
        surface.blit(pygame.transform.scale(item.image, (image_size, image_size)),
                     (offset_x + rect.x,
                      offset_y + rect.y))
        
        if event == ButtonEvent.Hover:
            surface.blit(global_var.Font.render(item.description, True, (255, 255, 255, 255)),
                         (rect.x + padding, rect.y + rect.h - global_var.Font.get_height()))
